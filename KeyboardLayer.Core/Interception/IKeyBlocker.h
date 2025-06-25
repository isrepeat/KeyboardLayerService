#pragma once
#include "3rdParty/Interception/interception.h"
#include "DeviceInfo.h"

namespace Interception {
	class IKeyBlocker {
	public:
		virtual bool ShouldBlock(const DeviceInfo& device, const InterceptionKeyStroke& stroke) const = 0;
		virtual ~IKeyBlocker() = default;
	};
}