#include "SimpleDeviceFilter.h"

namespace Interception {
	SimpleDeviceFilter::SimpleDeviceFilter(std::wstring requiredSubstring)
		: requiredSubstring{ std::move(requiredSubstring) }
	{}

	bool SimpleDeviceFilter::Accept(const DeviceInfo& device) const {
		if (this->requiredSubstring == L"any") {
			return true;
		}
		return device.hardwareId.find(this->requiredSubstring) != std::wstring::npos;
	}
}