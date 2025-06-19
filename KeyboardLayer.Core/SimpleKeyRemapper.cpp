#include "SimpleKeyRemapper.h"

SimpleKeyRemapper::SimpleKeyRemapper(std::map<LogicalKey, LogicalKey> keyMap, std::shared_ptr<IKeyCodeMapper> keyCodeMapper)
	: keyMap(std::move(keyMap))
	, keyCodeMapper(std::move(keyCodeMapper)) 
{}

bool SimpleKeyRemapper::ShouldRemap(const DeviceInfo& device, const InterceptionKeyStroke& stroke) const {
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