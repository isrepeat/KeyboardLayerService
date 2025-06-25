#include "KeyboardLayerEngine.h"
#include <Helpers/Logger.h>

#include "Keyboard/Core/LogicalKey.h"
#include "Interception/InterceptionKeyCodeMapper.h"
#include <iostream>


void SendCharacterW(wchar_t ch) {
	INPUT input[2]{};

	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0;
	input[0].ki.wScan = ch;
	input[0].ki.dwFlags = KEYEVENTF_UNICODE;

	input[1] = input[0];
	input[1].ki.dwFlags |= KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;

	SendInput(2, input, sizeof(INPUT));
}

KeyboardLayerEngine::KeyboardLayerEngine(
	std::unique_ptr<Interception::IKeyBlocker> keyBlocker,
	std::unique_ptr<Interception::IKeyRemapper> keyRemapper)
	: keyBlocker{ std::move(keyBlocker) }
	, keyRemapper{ std::move(keyRemapper) }
{
	this->context = interception_create_context();
	//this->keyInjector = std::make_unique<SimpleKeyInjector>(this->context);
	interception_set_filter(this->context, interception_is_keyboard, INTERCEPTION_FILTER_KEY_ALL);
}

void KeyboardLayerEngine::Run(std::stop_token stopToken) {
	LOG_FUNCTION_SCOPE("Run()");

	// где-то в начале метода:
	const int scanCodeTilda = 0x29; // физическая клавиша тильда (левая верхняя)
	const int scanCodeShift = 0x2A; // левый SHIFT

	auto keyCodeMapper = std::make_shared<Interception::InterceptionKeyCodeMapper>();

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
		auto deviceInfo = this->GetDeviceInfo(device);

		LOG_DEBUG_D("Key code: {}", key.code);


		//if (key.code == scanCodeTilda && key.state == INTERCEPTION_KEY_DOWN) {
		//	LOG_DEBUG_D("[InjectChar] Replacing key code {} with '~' symbol", key.code);

		//	SendCharacterW(L'`');
		//	continue;
		//}

		//// Подмена клавиш
		//if (this->keyRemapper && this->keyRemapper->ShouldRemap(deviceInfo, key)) {
		//	auto remapped = this->keyRemapper->Remap(key);

		//	// ⚠️ ВАЖНО:
		//	// Если ты подменяешь клавишу, обязательно обрабатывай и нажатие (key down), и отпускание (key up).
		//	// Иначе система подумает, что клавиша осталась "зажатой", если ты отправил только нажатие или только отпускание.
		//	// См. remapped.state — он должен совпадать с оригиналом.
		//	interception_send(this->context, device, (const InterceptionStroke*)&remapped, 1);

		//	LOG_DEBUG_D("[Remapped] Key code: {}  => {} on device {}"
		//		, key.code
		//		, remapped.code
		//		, device
		//	);
		//	continue;
		//}

		//// Блокировка клавиш
		//if (this->keyFilter && this->keyFilter->ShouldBlock(deviceInfo, key)) {
		//	LOG_DEBUG_D("[Blocked] Key code: {} from device {}"
		//		, key.code
		//		, device
		//	);
		//	continue;
		//}


	    // ➤ CapsLock-виртуальный SHIFT только для букв
		auto logicalOpt = keyCodeMapper->FromNative(key.code);
		if (logicalOpt &&
			*logicalOpt >= Keyboard::Core::Enums::LogicalKey::A &&
			*logicalOpt <= Keyboard::Core::Enums::LogicalKey::Z
			) {
			bool isCapsLockActive = (GetKeyState(VK_CAPITAL) & 0x0001) != 0;
			bool isShiftPressed = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;

			if (isCapsLockActive && !isShiftPressed) {
				// Принудительно добавим SHIFT
				// Windows будет считать, что SHIFT нажат

				if (key.state == InterceptionKeyState::INTERCEPTION_KEY_DOWN) {
					{
						INPUT input;
						input.type = INPUT_KEYBOARD;
						input.ki.wVk = VK_SHIFT;
						input.ki.dwFlags = 0;
						input.ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;
						SendInput(1, &input, sizeof(INPUT));
					}
					{
						INPUT input;
						input.type = INPUT_KEYBOARD;
						input.ki.wVk = 0x47;
						input.ki.dwFlags = 0;
						SendInput(1, &input, sizeof(INPUT));
					}
					//interception_send(this->context, device, &stroke, 1);
					{
						INPUT input;
						input.type = INPUT_KEYBOARD;
						input.ki.wVk = VK_SHIFT;
						input.ki.dwFlags = KEYEVENTF_KEYUP;
						input.ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;
						SendInput(1, &input, sizeof(INPUT));
					}
				}


				//interception_send(this->context, device, &stroke, 1);

				if (key.state == InterceptionKeyState::INTERCEPTION_KEY_UP) {
					{
						INPUT input;
						input.type = INPUT_KEYBOARD;
						input.ki.wVk = 0x47;
						input.ki.dwFlags = KEYEVENTF_KEYUP;
						SendInput(1, &input, sizeof(INPUT));
					}
					//interception_send(this->context, device, &stroke, 1);
				}

				continue;
			}
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