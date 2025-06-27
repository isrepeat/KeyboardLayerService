#pragma once
#include <Windows.h>

namespace Keyboard::Platform {
	class ModifierState {
	public:
		// ���������, ������� �� CapsLock (������� ����� ��������� ����).
		// ������������ GetKeyState, ��� ��� ������ �� ���������� toggle-���� (��� 0).
		// CapsLock ��������� ��������, ���� ��� 0 ����� 1.
		static bool IsCapsLockActive();

		// ���������, ������ �� ������� Shift (����� ��� ������).
		// ������������ GetAsyncKeyState ��� ��������� ����������� ��������� �������.
		static bool IsShiftPressed();

		// ���������, ������ �� ������� Ctrl (����� ��� ������).
		static bool IsCtrlPressed();

		// ���������, ������ �� ������� Alt (����� ��� ������).
		static bool IsAltPressed();
	};
}