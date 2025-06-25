#pragma once
#include <3rdParty/Interception/interception.h>
#include <string>

namespace Interception {
	struct DeviceInfo {
		InterceptionDevice device;
		std::wstring hardwareId;
	};
}