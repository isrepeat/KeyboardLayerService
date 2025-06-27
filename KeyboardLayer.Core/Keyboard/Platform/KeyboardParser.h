#pragma once
#include "KeyboardDeviceInfo.h"
#include <optional>
#include <string>

namespace Keyboard::Platform {
	class KeyboardParser {
	public:
		static std::optional<AcpiKeyboardInfo> TryParseAcpiKeyboard(const KeyboardDeviceInfo& baseKeyboardDeviceInfo);
		static std::optional<HidKeyboardInfo> TryParseHidKeyboard(const KeyboardDeviceInfo& baseKeyboardDeviceInfo);
	};
}