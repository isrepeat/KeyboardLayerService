#pragma once
#include <Windows.h>

namespace Keyboard::Platform {
	class Win32KeySender {
	public:
		void PressKey(WORD vk);
		void ReleaseKey(WORD vk);
	};
}