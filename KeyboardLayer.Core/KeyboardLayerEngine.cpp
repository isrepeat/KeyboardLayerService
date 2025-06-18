#include "KeyboardLayerEngine.h"
#include <iostream>

KeyboardLayerEngine::KeyboardLayerEngine(std::unique_ptr<KeyFilter> filter)
	: keyFilter(std::move(filter)) {
	context = interception_create_context();
	interception_set_filter(context, interception_is_keyboard, INTERCEPTION_FILTER_KEY_ALL);
}

std::wstring KeyboardLayerEngine::GetHardwareId(int device) {
	wchar_t id[512]{};
	interception_get_hardware_id(context, device, id, sizeof(id));
	return id;
}

DeviceInfo KeyboardLayerEngine::GetDeviceInfo(int device) {
	return DeviceInfo{ device, GetHardwareId(device) };
}

void KeyboardLayerEngine::Run() {
	std::wcout << L"[KeyboardLayer] Running...\n";

	while (true) {
		InterceptionDevice device = interception_wait(context);
		if (interception_is_keyboard(device)) {
			InterceptionStroke stroke;
			if (interception_receive(context, device, &stroke, 1) > 0) {
				InterceptionKeyStroke& key = (InterceptionKeyStroke&)stroke;
				DeviceInfo info = GetDeviceInfo(device);

				if (keyFilter->ShouldBlock(info, key)) {
					std::wcout << L"[Blocked] key code " << key.code << L" from device " << device << L"\n";
					continue;
				}

				interception_send(context, device, &stroke, 1);
			}
		}
	}
}