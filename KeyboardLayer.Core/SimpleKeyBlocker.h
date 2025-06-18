#pragma once
#include "KeyFilter.h"
#include <set>

class SimpleKeyBlocker : public KeyFilter {
public:
	SimpleKeyBlocker(const std::wstring& blockedDeviceIdPart, std::set<int> blockedKeys)
		: deviceIdPart(blockedDeviceIdPart), keysToBlock(std::move(blockedKeys)) {}

	bool ShouldBlock(const DeviceInfo& device, const InterceptionKeyStroke& stroke) const override {
		if (device.hardwareId.find(deviceIdPart) != std::wstring::npos) {
			return keysToBlock.contains(stroke.code);
		}
		return false;
	}

private:
	std::wstring deviceIdPart;
	std::set<int> keysToBlock;
};