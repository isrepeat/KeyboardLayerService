#pragma once
#include "DeviceInfo.h"

namespace Interception {
	class IDeviceFilter {
	public:
		virtual ~IDeviceFilter() = default;
		virtual bool Accept(const DeviceInfo& device) const = 0;
	};
}