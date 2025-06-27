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

	void Win32KeySender::SendUnicodeCharKeyDown(wchar_t ch) {
		INPUT input{};
		input.type = INPUT_KEYBOARD;
		input.ki.wVk = 0;
		input.ki.wScan = ch;
		input.ki.dwFlags = KEYEVENTF_UNICODE;

		SendInput(1, &input, sizeof(INPUT));
	}

	void Win32KeySender::SendUnicodeCharKeyUp(wchar_t ch) {
		INPUT input{};
		input.type = INPUT_KEYBOARD;
		input.ki.wVk = 0;
		input.ki.wScan = ch;
		input.ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;

		SendInput(1, &input, sizeof(INPUT));
	}
}