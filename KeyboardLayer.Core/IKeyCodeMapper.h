#pragma once
#include "LogicalKey.h"
#include <optional>

class IKeyCodeMapper {
public:
	virtual std::optional<int> ToNative(LogicalKey logicalKey) const = 0;
	virtual std::optional<LogicalKey> FromNative(int nativeCode) const = 0;
	virtual ~IKeyCodeMapper() = default;
};
