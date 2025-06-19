#pragma once
#include "InterceptionKeyCode.h"
#include "IKeyCodeMapper.h"

class InterceptionKeyCodeMapper : public IKeyCodeMapper {
public:
	std::optional<int> ToNative(LogicalKey logicalKey) const override;
	std::optional<LogicalKey> FromNative(int nativeCode) const override;
};