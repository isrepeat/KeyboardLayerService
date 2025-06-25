#pragma once
#include <3rdParty/Interception/interception.h>
#include "Interception/DeviceInfo.h"

namespace Interception::Rules {
	class IKeyRule {
	public:
		virtual ~IKeyRule() = default;
		virtual bool IsMatch(const DeviceInfo& device, const InterceptionKeyStroke& keyStroke) const = 0;
	};
}