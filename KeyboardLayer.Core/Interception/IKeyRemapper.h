#pragma once
#include "3rdParty/Interception/interception.h"
#include "DeviceInfo.h"

namespace Interception {
	class IKeyRemapper {
	public:
		virtual bool ShouldRemap(const DeviceInfo& device, const InterceptionKeyStroke& stroke) const = 0;
		virtual InterceptionKeyStroke Remap(const InterceptionKeyStroke& original) const = 0;
		virtual ~IKeyRemapper() = default;
	};
}