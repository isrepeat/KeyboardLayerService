#include "KeyboardLayerEngine.h"
#include <Helpers/Logger.h>

#include "Interception/InterceptionKeyCodeMapper.h"
#include "Keyboard/Platform/KeyboardEnumerator.h"
#include "Keyboard/Platform/KeyboardParser.h"
#include "Keyboard/Core/LogicalKey.h"

#include <iostream>


KeyboardLayerEngine::KeyboardLayerEngine(std::vector<std::shared_ptr<Interception::KeyProcessor>> keyProcessors)
	: keyProcessors{ std::move(keyProcessors) }
{
	this->context = interception_create_context();
	interception_set_filter(this->context, interception_is_keyboard, INTERCEPTION_FILTER_KEY_ALL);
}

void KeyboardLayerEngine::Run(std::stop_token stopToken) {
	LOG_FUNCTION_SCOPE("Run()");

	while (!stopToken.stop_requested()) {
		InterceptionDevice device = interception_wait_with_timeout(this->context, 500);
		if (device == 0) {
			continue; // timeout, нет ввода
		}

		if (!interception_is_keyboard(device)) {
			continue;
		}

		InterceptionStroke stroke;
		if (interception_receive(this->context, device, &stroke, 1) <= 0) {
			continue;
		}

		InterceptionKeyStroke& keyStroke = (InterceptionKeyStroke&)stroke;
		auto deviceInfo = this->GetDeviceInfo(device);

		LOG_DEBUG_D(L"Key code: {} [device = {}]"
			, keyStroke.code
			, deviceInfo.keyboardDeviceInfo ? deviceInfo.keyboardDeviceInfo->hardwareId : L"unknown"
		);


		bool isAnyProcessorWasApplied = this->ApplyKeyProcessors(deviceInfo, keyStroke);
		if (isAnyProcessorWasApplied) {
			continue;
		}

		// Передаём как есть
		interception_send(this->context, device, &stroke, 1);
	}

	interception_destroy_context(this->context);
}


Interception::DeviceInfo KeyboardLayerEngine::GetDeviceInfo(InterceptionDevice device) {
	std::wstring hardwareId = this->GetHardwareId(device);

	// Сначала ищем в кеше по исходному hardwareId
	auto it = this->cachedDeviceInfos.find(hardwareId);
	if (it != this->cachedDeviceInfos.end()) {
		return it->second;
	}

	// Преобразуем в upper-case только для поиска в KeyboardEnumerator
	std::wstring hardwareIdUpper = hardwareId;
	std::transform(hardwareIdUpper.begin(), hardwareIdUpper.end(), hardwareIdUpper.begin(), ::towupper);

	
	// Here hardwareIdUpper == ACPI\VEN_ATK&DEV_3001
	// but keyboardDevice.hardwareId = ACPI\\ATK3001
	Keyboard::Platform::KeyboardDeviceInfo keyboardDeviceInfo;
	keyboardDeviceInfo.instanceId = hardwareIdUpper;

	std::shared_ptr<Keyboard::Platform::KeyboardDeviceInfo> currentKeyboardDevice;

	if (auto acpiKeyboardDeviceInfo = Keyboard::Platform::KeyboardParser::TryParseAcpiKeyboard(keyboardDeviceInfo)) {
		currentKeyboardDevice = std::static_pointer_cast<Keyboard::Platform::KeyboardDeviceInfo>(
			std::make_shared<Keyboard::Platform::AcpiKeyboardInfo>(*acpiKeyboardDeviceInfo)
		);
	}
	else if (auto hidKeyboardDeviceInfo = Keyboard::Platform::KeyboardParser::TryParseHidKeyboard(keyboardDeviceInfo)) {
		currentKeyboardDevice = std::static_pointer_cast<Keyboard::Platform::KeyboardDeviceInfo>(
			std::make_shared<Keyboard::Platform::HidKeyboardInfo>(*hidKeyboardDeviceInfo)
		);
	}

	Interception::DeviceInfo deviceInfo;
	deviceInfo.interceptionDevice = device;

	auto allKeyboardDevices = Keyboard::Platform::KeyboardEnumerator::GetInstance().GetAllKeyboardDevices();
	if (allKeyboardDevices.contains(currentKeyboardDevice)) {
		deviceInfo.keyboardDeviceInfo = currentKeyboardDevice;
	}

	this->cachedDeviceInfos[hardwareId] = deviceInfo;
	return deviceInfo;
}


std::wstring KeyboardLayerEngine::GetHardwareId(InterceptionDevice device) {
	wchar_t id[512]{};
	interception_get_hardware_id(this->context, device, id, sizeof(id));
	return std::wstring(id);
}


bool KeyboardLayerEngine::ApplyKeyProcessors(Interception::DeviceInfo deviceInfo, InterceptionKeyStroke& keyStrokeRef) {
	bool isAnyActionWasApplied = false;

	for (const auto& processor : this->keyProcessors) {
		auto actionResult = processor->Process(deviceInfo, keyStrokeRef);
		auto processorChainPolicy = processor->GetChainPolicy();

		switch (actionResult) {
		case Enums::ActionResult::NotHandled:
			break;

		case Enums::ActionResult::Handled:
			if (processorChainPolicy == Interception::KeyProcessor::ChainPolicy::StopOnHandled) {
				return true; // действие было применено и оно блокирует дальнейшую обработку
			}
			else if (processorChainPolicy == Interception::KeyProcessor::ChainPolicy::AlwaysContinue) {
				isAnyActionWasApplied = true;
			}
		}
	}

	return isAnyActionWasApplied;
}