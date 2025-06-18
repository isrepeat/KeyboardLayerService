#pragma once
#include "3rdParty/Interception/interception.h"
#include "DeviceInfo.h"

class KeyFilter {
public:
	virtual bool ShouldBlock(const DeviceInfo& device, const InterceptionKeyStroke& stroke) const = 0;
	virtual ~KeyFilter() = default;
};