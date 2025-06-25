#pragma once
#include "IDeviceFilter.h"
#include <string>

namespace Interception {
	class SimpleDeviceFilter : public IDeviceFilter {
	public:
		explicit SimpleDeviceFilter(std::wstring requiredSubstring);
		bool Accept(const DeviceInfo& device) const override;

	private:
		std::wstring requiredSubstring;
	};
}