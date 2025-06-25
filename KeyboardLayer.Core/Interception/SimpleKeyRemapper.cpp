#include "SimpleKeyRemapper.h"
#include "Interception/InterceptionKeyCodeMapper.h"


namespace Interception {
	SimpleKeyRemapper::SimpleKeyRemapper(
		std::wstring deviceIdMatch,
		std::map<Keyboard::Core::Enums::LogicalKey, Keyboard::Core::Enums::LogicalKey> keyMap)
		: deviceIdMatch{ deviceIdMatch }
		, keyMap{ std::move(keyMap) }
		, keyCodeMapper{ std::make_unique<Interception::InterceptionKeyCodeMapper>() } {}

	bool SimpleKeyRemapper::ShouldRemap(const DeviceInfo& device, const InterceptionKeyStroke& stroke) const {
		//if (device.hardwareId.find(this->deviceIdMatch) == std::wstring::npos) {
		//	return false;
		//}
		auto logical = this->keyCodeMapper->FromNative(stroke.code);
		return logical && this->keyMap.contains(*logical);
	}

	InterceptionKeyStroke SimpleKeyRemapper::Remap(const InterceptionKeyStroke& original) const {
		InterceptionKeyStroke remapped = original;
		auto logical = this->keyCodeMapper->FromNative(original.code);
		if (logical && this->keyMap.contains(*logical)) {
			auto toLogical = keyMap.at(*logical);
			auto native = this->keyCodeMapper->ToNative(toLogical);
			if (native) {
				remapped.code = *native;
			}
		}
		return remapped;
	}
}