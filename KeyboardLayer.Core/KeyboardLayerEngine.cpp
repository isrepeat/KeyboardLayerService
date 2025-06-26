#include "KeyboardLayerEngine.h"
#include <Helpers/Logger.h>

#include "Interception/InterceptionKeyCodeMapper.h"
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
		LOG_DEBUG_D("Key code: {}", keyStroke.code);

		auto deviceInfo = this->GetDeviceInfo(device);

		bool isAnyProcessorWasApplied = this->ApplyKeyProcessors(deviceInfo, keyStroke);
		if (isAnyProcessorWasApplied) {
			continue;
		}

		// Передаём как есть
		interception_send(this->context, device, &stroke, 1);
	}

	interception_destroy_context(this->context);
}


std::wstring KeyboardLayerEngine::GetHardwareId(int device) {
	wchar_t id[512]{};
	interception_get_hardware_id(this->context, device, id, sizeof(id));
	return std::wstring(id);
}


Interception::DeviceInfo KeyboardLayerEngine::GetDeviceInfo(int device) {
	return Interception::DeviceInfo{ device, this->GetHardwareId(device) };
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