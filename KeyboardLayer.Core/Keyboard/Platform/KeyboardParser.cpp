#include "KeyboardParser.h"
#include <regex>

namespace Keyboard::Platform {
	std::optional<AcpiKeyboardInfo> KeyboardParser::TryParseAcpiKeyboard(const KeyboardDeviceInfo& keyboardDeviceInfo) {
		// ������: ACPI\VEN_ATK&DEV_3001
		// �������� ������������ ACPI/PNP, VEN � ��� 3-���������� ��������-�������� ��� �������������:
		// VEN_ATK -> ASUS
		// VEN_DLL -> Dell
		// VEN_HPQ -> HP
		// VEN_LEN -> Lenovo
		std::wregex regexFull(LR"(ACPI\\VEN_([A-Z0-9]{3})&DEV_([A-Z0-9]{4}))", std::regex::icase);

		// �������������� �������� ������: ACPI\ATK3001
		std::wregex regexShort(LR"(ACPI\\([A-Z0-9]{3})([0-9]{4}))", std::regex::icase);

		std::wsmatch match;

		// ������� ���������� ������ ����� (� VEN_ � DEV_)
		if (std::regex_search(keyboardDeviceInfo.instanceId, match, regexFull) && match.size() == 3) {
			AcpiKeyboardInfo info;
			info.instanceId = keyboardDeviceInfo.instanceId;
			info.hardwareId = match[0].str(); // "ACPI\\VEN_ATK&DEV_3001"
			info.venCode = match[1].str(); // "ATK"
			info.devCode = match[2].str(); // "3001"
			return info;

		}

		// ������� ���������� ����������� �����: ACPI\ATK3001
		if (std::regex_search(keyboardDeviceInfo.instanceId, match, regexShort) && match.size() == 3) {
			AcpiKeyboardInfo info;
			info.instanceId = keyboardDeviceInfo.instanceId;
			info.hardwareId = match[0].str(); // "ACPI\\ATKDEV_3001"
			info.venCode = match[1].str(); // "ATK"
			info.devCode = match[2].str(); // "3001"
			return info;
		}

		return std::nullopt;
	}

	std::optional<HidKeyboardInfo> KeyboardParser::TryParseHidKeyboard(const KeyboardDeviceInfo& keyboardDeviceInfo) {
		// ��������� ��� HID ���������:
		// ������� �����:
		// HID\VID_046D&PID_C31C&COL01\7&XYZ&0&0000
		// HID\{GUID}_VID&0002_PID&7021&COL01\8&XYZ&F&0000

		// ������:
		// [1] � Bluetooth GUID (�����������)
		// [2] � VID
		// [3] � PID
		// [4] � COL index (�����������)
		std::wregex regexHid(LR"(HID\\(?:\{([0-9A-F\-]{36})\}_)?VID[_&]?([0-9A-F]{4,8})[_&]PID[_&]?([0-9A-F]{4,8})(?:[_&]COL(\d{2}))?)", std::regex::icase);

		std::wsmatch match;
		if (std::regex_search(keyboardDeviceInfo.instanceId, match, regexHid)) {
			HidKeyboardInfo info;

			info.instanceId = keyboardDeviceInfo.instanceId;
			info.hardwareId = match[0].str(); // ���������� �����: HID\VID_xxxx&PID_xxxx&COLxx (��� � GUID)

			// Bluetooth GUID (���� ���)
			info.btGuid = match[1].matched
				? std::make_optional(match[1].str())
				: std::nullopt;

			// Vendor ID (VID)
			info.vid = match[2].str();

			// Product ID (PID)
			info.pid = match[3].str();

			// Collection index (�������� COL01)
			info.collectionIndex = match[4].matched
				? std::make_optional(match[4].str())
				: std::nullopt;

			return info;
		}

		return std::nullopt;
	}
}