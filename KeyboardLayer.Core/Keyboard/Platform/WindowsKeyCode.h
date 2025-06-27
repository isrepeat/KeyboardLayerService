#pragma once
#include <Windows.h>

namespace Keyboard::Platform {
	namespace Enums {
		enum class WindowsKeyCode : int {
			// Letters
			A = 0x41,
			B = 0x42,
			C = 0x43,
			D = 0x44,
			E = 0x45,
			F = 0x46,
			G = 0x47,
			H = 0x48,
			I = 0x49,
			J = 0x4A,
			K = 0x4B,
			L = 0x4C,
			M = 0x4D,
			N = 0x4E,
			O = 0x4F,
			P = 0x50,
			Q = 0x51,
			R = 0x52,
			S = 0x53,
			T = 0x54,
			U = 0x55,
			V = 0x56,
			W = 0x57,
			X = 0x58,
			Y = 0x59,
			Z = 0x5A,

			// Digits
			Digit0 = 0x30,
			Digit1 = 0x31,
			Digit2 = 0x32,
			Digit3 = 0x33,
			Digit4 = 0x34,
			Digit5 = 0x35,
			Digit6 = 0x36,
			Digit7 = 0x37,
			Digit8 = 0x38,
			Digit9 = 0x39,

			// Punctuation
			Minus = VK_OEM_MINUS,
			Plus = VK_OEM_PLUS,
			Tilde = VK_OEM_3,
			LeftBracket = VK_OEM_4,
			RightBracket = VK_OEM_6,
			Backslash = VK_OEM_5,
			Semicolon = VK_OEM_1,
			Apostrophe = VK_OEM_7,
			Comma = VK_OEM_COMMA,
			Period = VK_OEM_PERIOD,
			Slash = VK_OEM_2,

			// Control keys
			Enter = VK_RETURN,
			Escape = VK_ESCAPE,
			Backspace = VK_BACK,
			Tab = VK_TAB,
			Space = VK_SPACE,
			CapsLock = VK_CAPITAL,
			Insert = VK_INSERT,
			Delete = VK_DELETE,

			// Function keys
			F1 = VK_F1,
			F2 = VK_F2,
			F3 = VK_F3,
			F4 = VK_F4,
			F5 = VK_F5,
			F6 = VK_F6,
			F7 = VK_F7,
			F8 = VK_F8,
			F9 = VK_F9,
			F10 = VK_F10,
			F11 = VK_F11,
			F12 = VK_F12,

			// Navigation
			ArrowUp = VK_UP,
			ArrowDown = VK_DOWN,
			ArrowLeft = VK_LEFT,
			ArrowRight = VK_RIGHT,
			Home = VK_HOME,
			End = VK_END,
			PageUp = VK_PRIOR,
			PageDown = VK_NEXT,

			// Numpad
			NumLock = VK_NUMLOCK,
			NumpadPlus = VK_ADD,
			NumpadMinus = VK_SUBTRACT,
			NumpadAsterisk = VK_MULTIPLY,
			NumpadSlash = VK_DIVIDE,
			Numpad0 = VK_NUMPAD0,
			Numpad1 = VK_NUMPAD1,
			Numpad2 = VK_NUMPAD2,
			Numpad3 = VK_NUMPAD3,
			Numpad4 = VK_NUMPAD4,
			Numpad5 = VK_NUMPAD5,
			Numpad6 = VK_NUMPAD6,
			Numpad7 = VK_NUMPAD7,
			Numpad8 = VK_NUMPAD8,
			Numpad9 = VK_NUMPAD9,
			NumpadDot = VK_DECIMAL,
			NumpadEnter = VK_RETURN,

			// Modifiers
			LeftCtrl = VK_LCONTROL,
			RightCtrl = VK_RCONTROL,
			LeftShift = VK_LSHIFT,
			RightShift = VK_RSHIFT,
			LeftAlt = VK_LMENU,
			RightAlt = VK_RMENU,
			LeftWin = VK_LWIN,
			RightWin = VK_RWIN,
		};
	}
}