#pragma once
#include "IKeyCodeMapper.h"
#include "IKeyFilter.h"
#include <memory>
#include <set>

class SimpleKeyBlocker : public IKeyFilter {
public:
	SimpleKeyBlocker(
		std::wstring deviceIdMatch,
		std::set<LogicalKey> logicalKeysToBlock,
		std::shared_ptr<IKeyCodeMapper> keyCodeMapper);

	bool ShouldBlock(const DeviceInfo& device, const InterceptionKeyStroke& stroke) const override;

private:
	std::wstring deviceIdPart;
	std::set<LogicalKey> keysToBlock;
	std::shared_ptr<IKeyCodeMapper> keyCodeMapper;
};