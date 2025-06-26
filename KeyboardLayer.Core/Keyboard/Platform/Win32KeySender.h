#pragma once
#include <Windows.h>

namespace Keyboard::Platform {
	class Win32KeySender {
	public:
		void PressKey(WORD vk);
		void ReleaseKey(WORD vk);

		void SendUnicodeCharKeyDown(wchar_t ch);
		void SendUnicodeCharKeyUp(wchar_t ch);
	};
}