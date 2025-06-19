#include "SimpleKeyBlocker.h"

SimpleKeyBlocker::SimpleKeyBlocker(
	std::wstring deviceIdMatch,
	std::set<LogicalKey> logicalKeysToBlock,
	std::shared_ptr<IKeyCodeMapper> keyCodeMapper)
	: deviceIdPart(std::move(deviceIdMatch))
	, keysToBlock(std::move(logicalKeysToBlock))
	, keyCodeMapper(std::move(keyCodeMapper))
{}

bool SimpleKeyBlocker::ShouldBlock(const DeviceInfo& device, const InterceptionKeyStroke& stroke) const {
	//if (device.hardwareId.find(this->deviceIdPart) == std::wstring::npos) {
	//	return false;
	//}
	auto logical = this->keyCodeMapper->FromNative(stroke.code);
	return logical && this->keysToBlock.contains(*logical);
}