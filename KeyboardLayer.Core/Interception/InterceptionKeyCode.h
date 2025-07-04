#pragma once

namespace Interception {
	namespace Enums {
		enum class InterceptionKeyCode {
			// Letters
			A = 0x1E,
			B = 0x30,
			C = 0x2E,
			D = 0x20,
			E = 0x12,
			F = 0x21,
			G = 0x22,
			H = 0x23,
			I = 0x17,
			J = 0x24,
			K = 0x25,
			L = 0x26,
			M = 0x32,
			N = 0x31,
			O = 0x18,
			P = 0x19,
			Q = 0x10,
			R = 0x13,
			S = 0x1F,
			T = 0x14,
			U = 0x16,
			V = 0x2F,
			W = 0x11,
			X = 0x2D,
			Y = 0x15,
			Z = 0x2C,
			LeftBracket = 0x1A,
			RightBracket = 0x1B,
			Backslash = 0x2B,
			Slash = 0x35,
			Semicolon = 0x27,
			Apostrophe = 0x28,
			Comma = 0x33,
			Period = 0x34,
			Oem3 = 0x29, // Tilde

			// Digits
			Digit1 = 0x02,
			Digit2 = 0x03,
			Digit3 = 0x04,
			Digit4 = 0x05,
			Digit5 = 0x06,
			Digit6 = 0x07,
			Digit7 = 0x08,
			Digit8 = 0x09,
			Digit9 = 0x0A,
			Digit0 = 0x0B,
			Minus = 0x0C,
			Plus = 0x0D,

			// Controls
			Enter = 0x1C,
			Escape = 0x01,
			Backspace = 0x0E,
			Tab = 0x0F,
			Space = 0x39,
			CapsLock = 0x3A,
			Insert = 0x52,
			Delete = 0x53,

			// Function keys
			F1 = 0x3B,
			F2 = 0x3C,
			F3 = 0x3D,
			F4 = 0x3E,
			F5 = 0x3F,
			F6 = 0x40,
			F7 = 0x41,
			F8 = 0x42,
			F9 = 0x43,
			F10 = 0x44,
			F11 = 0x57,
			F12 = 0x58,

			// Arrows and navigation
			ArrowUp = 0x48,
			ArrowDown = 0x50,
			ArrowLeft = 0x4B,
			ArrowRight = 0x4D,
			Home = 0x47,
			End = 0x4F,
			PageUp = 0x49,
			PageDown = 0x51,

			//// Arrows and navigation (extended codes)
			//Insert = 0xD2,
			//Delete = 0xD3,
			//Home = 0xC7,
			//End = 0xCF,
			//PageUp = 0xC9,
			//PageDown = 0xD1,
			//ArrowUp = 0xC8,
			//ArrowDown = 0xD0,
			//ArrowLeft = 0xCB,
			//ArrowRight = 0xCD,

			// Numpad
			NumLock = 0x45,
			NumpadPlus = 0x4E,
			NumpadMinus = 0x4A,
			NumpadAsterisk = 0x37,
			NumpadSlash = 0xB5,
			Numpad1 = 0x4F, // End
			Numpad2 = 0x50, // ArrowDown
			Numpad3 = 0x51, // PageDown
			Numpad4 = 0x4B, // ArrowLeft
			Numpad5 = 0x4C,
			Numpad6 = 0x4D, // ArrowRight
			Numpad7 = 0x47, // Home
			Numpad8 = 0x48, // ArrowUp
			Numpad9 = 0x49, // PageUp
			Numpad0 = 0x52, // Insert
			NumpadDot = 0x53, // Delete
			NumpadEnter = 0x9C,

			// Modifiers
			LeftCtrl = 0x1D,
			RightCtrl = 0x9D,
			LeftShift = 0x2A,
			RightShift = 0x36,
			LeftAlt = 0x38,
			RightAlt = 0xB8,
			LeftWin = 0x5B,
			RightWin = 0x5C,
		};
	}
}