#pragma once
#include <Windows.h>

namespace Keyboard::Platform {
	class ModifierState {
	public:
		// ѕровер€ет, активен ли CapsLock (включен режим заглавных букв).
		// »спользуетс€ GetKeyState, так как только он возвращает toggle-флаг (бит 0).
		// CapsLock считаетс€ активным, если бит 0 равен 1.
		static bool IsCapsLockActive();

		// ѕровер€ет, зажата ли клавиша Shift (лева€ или права€).
		// »спользуетс€ GetAsyncKeyState дл€ получени€ физического состо€ни€ клавиши.
		static bool IsShiftPressed();

		// ѕровер€ет, зажата ли клавиша Ctrl (лева€ или права€).
		static bool IsCtrlPressed();

		// ѕровер€ет, зажата ли клавиша Alt (лева€ или права€).
		static bool IsAltPressed();
	};
}