#include <Helpers/Logger.h>
#include "KeyboardLayerEngine.h"
#include <iostream>

KeyboardLayerEngine::KeyboardLayerEngine(
	std::unique_ptr<IKeyFilter> keyFilter,
	std::unique_ptr<IKeyRemapper> keyRemapper)
	: keyFilter(std::move(keyFilter))
	, keyRemapper(std::move(keyRemapper))
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

		InterceptionKeyStroke& key = (InterceptionKeyStroke&)stroke;
		DeviceInfo deviceInfo = this->GetDeviceInfo(device);

		LOG_DEBUG_D("Key code: {}", key.code);

		// Подмена клавиш
		if (this->keyRemapper && this->keyRemapper->ShouldRemap(deviceInfo, key)) {
			auto remapped = this->keyRemapper->Remap(key);

			// ⚠️ ВАЖНО:
			// Если ты подменяешь клавишу, обязательно обрабатывай и нажатие (key down), и отпускание (key up).
			// Иначе система подумает, что клавиша осталась "зажатой", если ты отправил только нажатие или только отпускание.
			// См. remapped.state — он должен совпадать с оригиналом.
			interception_send(this->context, device, (const InterceptionStroke*)&remapped, 1);

			LOG_DEBUG_D("[Remapped] Key code: {}  => {} on device {}"
				, key.code
				, remapped.code
				, device
			);
			continue;
		}

		// Блокировка клавиш
		if (this->keyFilter && this->keyFilter->ShouldBlock(deviceInfo, key)) {
			LOG_DEBUG_D("[Blocked] Key code: {} from device {}"
				, key.code
				, device
			);
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

DeviceInfo KeyboardLayerEngine::GetDeviceInfo(int device) {
	return DeviceInfo{ device, this->GetHardwareId(device) };
}