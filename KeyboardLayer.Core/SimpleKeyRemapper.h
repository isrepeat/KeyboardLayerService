#pragma once
#include "IKeyCodeMapper.h"
#include "IKeyRemapper.h"
#include <memory>
#include <map>

class SimpleKeyRemapper : public IKeyRemapper {
public:
	SimpleKeyRemapper(
		std::map<LogicalKey, LogicalKey> keyMap,
		std::shared_ptr<IKeyCodeMapper> keyCodeMapper);

	bool ShouldRemap(const DeviceInfo& device, const InterceptionKeyStroke& stroke) const override;
	InterceptionKeyStroke Remap(const InterceptionKeyStroke& original) const override;

private:
	std::map<LogicalKey, LogicalKey> keyMap;
	std::shared_ptr<IKeyCodeMapper> keyCodeMapper;
};