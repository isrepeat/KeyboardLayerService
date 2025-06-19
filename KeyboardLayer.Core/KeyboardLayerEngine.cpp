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

void KeyboardLayerEngine::Run() {
	std::wcout << L"[KeyboardLayer] Running...\n";

	while (true) {
		InterceptionDevice device = interception_wait(this->context);
		if (!interception_is_keyboard(device))
			continue;

		InterceptionStroke stroke;
		if (interception_receive(this->context, device, &stroke, 1) <= 0)
			continue;

		InterceptionKeyStroke& key = (InterceptionKeyStroke&)stroke;
		DeviceInfo deviceInfo = GetDeviceInfo(device);

		std::wcout << L"Key code: " << key.code << L"\n";


		// Подмена клавиш
		if (this->keyRemapper && this->keyRemapper->ShouldRemap(deviceInfo, key)) {
			auto remapped = this->keyRemapper->Remap(key);

			// ⚠️ ВАЖНО:
			// Если ты подменяешь клавишу, обязательно обрабатывай и нажатие (key down), и отпускание (key up).
			// Иначе система подумает, что клавиша осталась "зажатой", если ты отправил только нажатие или только отпускание.
			// См. remapped.state — он должен совпадать с оригиналом.
			interception_send(this->context, device, (const InterceptionStroke*)&remapped, 1);

			std::wcout << L"[Remapped] code " << key.code << L" => " << remapped.code << L" on device " << device << std::endl;
			continue;
		}

		// Блокировка клавиш
		if (this->keyFilter && this->keyFilter->ShouldBlock(deviceInfo, key)) {
			std::wcout << L"[Blocked] key code " << key.code << L" from device " << device << std::endl;
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
	return DeviceInfo{ device, GetHardwareId(device) };
}

