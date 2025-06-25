#include "ModifierState.h"

bool ModifierState::IsCapsLockActive() {
	// GetKeyState:
	// - бит 0 (LSB): toggle-состояние (вкл/выкл) — нужен именно он
	// - бит 15 (MSB): нажата ли клавиша (здесь не используется)
	return (GetKeyState(VK_CAPITAL) & 0x0001) != 0;
}

bool ModifierState::IsShiftPressed() {
	// GetAsyncKeyState:
	// - бит 15 (MSB): физически нажата ли клавиша (в момент вызова)
	// - надёжно работает даже вне фокуса окна (например, в фоне или с Interception)
	return (GetAsyncKeyState(VK_LSHIFT) & 0x8000) != 0 ||
		(GetAsyncKeyState(VK_RSHIFT) & 0x8000) != 0;
}

bool ModifierState::IsCtrlPressed() {
	return (GetAsyncKeyState(VK_LCONTROL) & 0x8000) != 0 ||
		(GetAsyncKeyState(VK_RCONTROL) & 0x8000) != 0;
}

bool ModifierState::IsAltPressed() {
	return (GetAsyncKeyState(VK_LMENU) & 0x8000) != 0 ||
		(GetAsyncKeyState(VK_RMENU) & 0x8000) != 0;
}