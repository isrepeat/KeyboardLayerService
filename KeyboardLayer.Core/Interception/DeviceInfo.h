#pragma once
#include <3rdParty/Interception/interception.h>
#include "Keyboard/Platform/KeyboardDeviceInfo.h"
#include <string>

namespace Interception {
	struct DeviceInfo {
		InterceptionDevice interceptionDevice;
		std::shared_ptr<Keyboard::Platform::KeyboardDeviceInfo> keyboardDeviceInfo;
	};
}