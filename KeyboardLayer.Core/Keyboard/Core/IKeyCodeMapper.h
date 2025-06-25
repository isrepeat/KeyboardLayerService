#pragma once
#include "LogicalKey.h"
#include <optional>

namespace Keyboard::Core {
	class IKeyCodeMapper {
	public:
		virtual std::optional<int> ToNative(Keyboard::Core::Enums::LogicalKey logicalKey) const = 0;
		virtual std::optional<Keyboard::Core::Enums::LogicalKey> FromNative(int nativeCode) const = 0;
		virtual ~IKeyCodeMapper() = default;
	};
}
