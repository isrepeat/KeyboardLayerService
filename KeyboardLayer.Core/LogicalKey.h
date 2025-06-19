#pragma once

enum class LogicalKey {
	A, B, C, D, E, F, G, H, I, J, K, L, M, N,
	O, P, Q, R, S, T, U, V, W, X, Y, Z, 
	LeftBracket, 
	RightBracket,
	Backslash,
	Slash,
	Semicolon,
	Apostrophe,
	Comma,
	Period,
	Tilde,

	Digit0, Digit1, Digit2, Digit3, Digit4,
	Digit5, Digit6, Digit7, Digit8, Digit9,
	Minus, Plus,

	Enter, Escape, Backspace, Tab, Space, CapsLock, Insert, Delete,

	F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

	ArrowUp, ArrowDown, ArrowLeft, ArrowRight, Home, End, PageUp, PageDown,

	NumLock,
	NumpadPlus,
	NumpadMinus,
	NumpadAsterisk,
	NumpadSlash,
	Numpad1, Numpad2, Numpad3, Numpad4, Numpad5,
	Numpad6, Numpad7, Numpad8, Numpad9, Numpad0,
	NumpadDot,
	NumpadEnter,

	LeftCtrl, RightCtrl, LeftShift, RightShift, LeftAlt, RightAlt, LeftWin, RightWin,
};