#include "Win32KeySender.h"

namespace Keyboard::Platform {
	void Win32KeySender::PressKey(WORD vk) {
		INPUT input{};
		input.type = INPUT_KEYBOARD;
		input.ki.wVk = vk;
		SendInput(1, &input, sizeof(INPUT));
	}

	void Win32KeySender::ReleaseKey(WORD vk) {
		INPUT input{};
		input.type = INPUT_KEYBOARD;
		input.ki.wVk = vk;
		input.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &input, sizeof(INPUT));
	}
}

//void KeyInjector::TapVirtualKey(WORD vk) {
//	this->PressKey(vk);
//	this->ReleaseKey(vk);
//}
//
//void KeyInjector::TapVirtualKeyWithShift(WORD vk) {
//	this->PressKey(VK_SHIFT);
//	this->TapVirtualKey(vk);
//	this->ReleaseKey(VK_SHIFT);
//}