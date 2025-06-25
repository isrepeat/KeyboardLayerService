#include "SimpleKeyBlocker.h"
#include "Interception/InterceptionKeyCodeMapper.h"

namespace Interception {
	SimpleKeyBlocker::SimpleKeyBlocker(
		std::wstring deviceIdMatch,
		std::set<Keyboard::Core::Enums::LogicalKey> logicalKeysToBlock)
		: deviceIdMatch{ deviceIdMatch }
		, keysToBlock{ std::move(logicalKeysToBlock) }
		, keyCodeMapper{ std::make_unique<Interception::InterceptionKeyCodeMapper>() }
	{}

	bool SimpleKeyBlocker::ShouldBlock(const DeviceInfo& device, const InterceptionKeyStroke& stroke) const {
		//if (device.hardwareId.find(this->deviceIdMatch) == std::wstring::npos) {
		//	return false;
		//}
		auto logical = this->keyCodeMapper->FromNative(stroke.code);
		return logical && this->keysToBlock.contains(*logical);
	}
}