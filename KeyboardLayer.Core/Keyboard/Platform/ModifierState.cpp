#include "ModifierState.h"

bool ModifierState::IsCapsLockActive() {
	// GetKeyState:
	// - ��� 0 (LSB): toggle-��������� (���/����) � ����� ������ ��
	// - ��� 15 (MSB): ������ �� ������� (����� �� ������������)
	return (GetKeyState(VK_CAPITAL) & 0x0001) != 0;
}

bool ModifierState::IsShiftPressed() {
	// GetAsyncKeyState:
	// - ��� 15 (MSB): ��������� ������ �� ������� (� ������ ������)
	// - ������ �������� ���� ��� ������ ���� (��������, � ���� ��� � Interception)
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