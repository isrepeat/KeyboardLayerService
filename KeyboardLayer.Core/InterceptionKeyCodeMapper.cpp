#include "InterceptionKeyCodeMapper.h"

std::optional<int> InterceptionKeyCodeMapper::ToNative(LogicalKey key) const {
	using LK = LogicalKey;
	using IK = InterceptionKeyCode;

	switch (key) {
#define MAP(lk, ik) case LK::lk: return static_cast<int>(IK::ik);

		MAP(A, A); MAP(B, B); MAP(C, C); MAP(D, D); MAP(E, E); MAP(F, F); MAP(G, G);
		MAP(H, H); MAP(I, I); MAP(J, J); MAP(K, K); MAP(L, L); MAP(M, M); MAP(N, N);
		MAP(O, O); MAP(P, P); MAP(Q, Q); MAP(R, R); MAP(S, S); MAP(T, T); MAP(U, U);
		MAP(V, V); MAP(W, W); MAP(X, X); MAP(Y, Y); MAP(Z, Z);
		MAP(LeftBracket, LeftBracket);
		MAP(RightBracket, RightBracket);
		MAP(Backslash, Backslash);
		MAP(Slash, Slash);
		MAP(Semicolon, Semicolon);
		MAP(Apostrophe, Apostrophe);
		MAP(Comma, Comma);
		MAP(Period, Period);
		MAP(Tilde, Oem3);

		MAP(Digit0, Digit0); MAP(Digit1, Digit1); MAP(Digit2, Digit2);
		MAP(Digit3, Digit3); MAP(Digit4, Digit4); MAP(Digit5, Digit5);
		MAP(Digit6, Digit6); MAP(Digit7, Digit7); MAP(Digit8, Digit8); 
		MAP(Digit9, Digit9); 
		MAP(Minus, Minus);
		MAP(Plus, Plus);

		MAP(Enter, Enter);
		MAP(Escape, Escape);
		MAP(Backspace, Backspace);
		MAP(Tab, Tab);
		MAP(Space, Space);
		MAP(CapsLock, CapsLock);
		MAP(Insert, Insert);
		MAP(Delete, Delete);

		MAP(F1, F1); MAP(F2, F2); MAP(F3, F3); MAP(F4, F4); MAP(F5, F5); MAP(F6, F6);
		MAP(F7, F7); MAP(F8, F8); MAP(F9, F9); MAP(F10, F10); MAP(F11, F11); MAP(F12, F12);

		MAP(ArrowUp, ArrowUp);
		MAP(ArrowDown, ArrowDown);
		MAP(ArrowLeft, ArrowLeft);
		MAP(ArrowRight, ArrowRight);
		MAP(Home, Home);
		MAP(End, End);
		MAP(PageUp, PageUp);
		MAP(PageDown, PageDown);

		MAP(NumLock, NumLock);
		MAP(NumpadPlus, NumpadPlus);
		MAP(NumpadMinus, NumpadMinus);
		MAP(NumpadAsterisk, NumpadAsterisk);
		MAP(NumpadSlash, NumpadSlash);
		//MAP(Numpad1, Numpad1); // End
		//MAP(Numpad2, Numpad2); // ArrowDown
		//MAP(Numpad3, Numpad3); // PageDown
		//MAP(Numpad4, Numpad4); // ArrowLeft
		MAP(Numpad5, Numpad5);
		//MAP(Numpad6, Numpad6); // ArrowRight
		//MAP(Numpad7, Numpad7); // Home
		//MAP(Numpad8, Numpad8); // ArrowUp
		//MAP(Numpad9, Numpad9); // PageUp
		//MAP(Numpad0, Numpad0); // Insert
		MAP(NumpadEnter, NumpadEnter);
		//MAP(NumpadDot, NumpadDot); // Delete

		MAP(LeftCtrl, LeftCtrl); MAP(RightCtrl, RightCtrl);
		MAP(LeftShift, LeftShift); MAP(RightShift, RightShift);
		MAP(LeftAlt, LeftAlt); MAP(RightAlt, RightAlt);
		MAP(LeftWin, LeftWin); MAP(RightWin, RightWin);

#undef MAP
	}

	return std::nullopt;
}

std::optional<LogicalKey> InterceptionKeyCodeMapper::FromNative(int code) const {
	using LK = LogicalKey;
	using IK = InterceptionKeyCode;

	switch (static_cast<IK>(code)) {
#define MAP(ik, lk) case IK::ik: return LK::lk;

		MAP(A, A); MAP(B, B); MAP(C, C); MAP(D, D); MAP(E, E); MAP(F, F); MAP(G, G);
		MAP(H, H); MAP(I, I); MAP(J, J); MAP(K, K); MAP(L, L); MAP(M, M); MAP(N, N);
		MAP(O, O); MAP(P, P); MAP(Q, Q); MAP(R, R); MAP(S, S); MAP(T, T); MAP(U, U);
		MAP(V, V); MAP(W, W); MAP(X, X); MAP(Y, Y); MAP(Z, Z); 
		MAP(LeftBracket, LeftBracket);
		MAP(RightBracket, RightBracket);
		MAP(Backslash, Backslash);
		MAP(Slash, Slash);
		MAP(Semicolon, Semicolon);
		MAP(Apostrophe, Apostrophe);
		MAP(Comma, Comma);
		MAP(Period, Period);
		MAP(Oem3, Tilde);

		MAP(Digit0, Digit0); MAP(Digit1, Digit1); MAP(Digit2, Digit2);
		MAP(Digit3, Digit3); MAP(Digit4, Digit4); MAP(Digit5, Digit5);
		MAP(Digit6, Digit6); MAP(Digit7, Digit7); MAP(Digit8, Digit8);
		MAP(Digit9, Digit9); 
		MAP(Minus, Minus);
		MAP(Plus, Plus);

		MAP(Enter, Enter);
		MAP(Escape, Escape);
		MAP(Backspace, Backspace);
		MAP(Tab, Tab);
		MAP(Space, Space);
		MAP(CapsLock, CapsLock);
		MAP(Insert, Insert);
		MAP(Delete, Delete);

		MAP(F1, F1); MAP(F2, F2); MAP(F3, F3); MAP(F4, F4); MAP(F5, F5); MAP(F6, F6);
		MAP(F7, F7); MAP(F8, F8); MAP(F9, F9); MAP(F10, F10); MAP(F11, F11); MAP(F12, F12);

		MAP(ArrowUp, ArrowUp);
		MAP(ArrowDown, ArrowDown);
		MAP(ArrowLeft, ArrowLeft);
		MAP(ArrowRight, ArrowRight);
		MAP(Home, Home);
		MAP(End, End);
		MAP(PageUp, PageUp);
		MAP(PageDown, PageDown);

		MAP(NumLock, NumLock);
		MAP(NumpadPlus, NumpadPlus);
		MAP(NumpadMinus, NumpadMinus);
		MAP(NumpadAsterisk, NumpadAsterisk);
		MAP(NumpadSlash, NumpadSlash);
		//MAP(Numpad1, Numpad1); // End
		//MAP(Numpad2, Numpad2); // ArrowDown
		//MAP(Numpad3, Numpad3); // PageDown
		//MAP(Numpad4, Numpad4); // ArrowLeft
		MAP(Numpad5, Numpad5);
		//MAP(Numpad6, Numpad6); // ArrowRight
		//MAP(Numpad7, Numpad7); // Home
		//MAP(Numpad8, Numpad8); // ArrowUp
		//MAP(Numpad9, Numpad9); // PageUp
		//MAP(Numpad0, Numpad0); // Insert
		MAP(NumpadEnter, NumpadEnter);
		//MAP(NumpadDot, NumpadDot); // Delete

		MAP(LeftCtrl, LeftCtrl); MAP(RightCtrl, RightCtrl);
		MAP(LeftShift, LeftShift); MAP(RightShift, RightShift);
		MAP(LeftAlt, LeftAlt); MAP(RightAlt, RightAlt);
		MAP(LeftWin, LeftWin); MAP(RightWin, RightWin);

#undef MAP
	}

	return std::nullopt;
}