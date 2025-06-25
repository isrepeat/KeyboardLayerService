#pragma once
#include "Keyboard/Core/IKeyCodeMapper.h"
#include <optional>

namespace Keyboard::Platform {
	class WindowsKeyCodeMapper : public Keyboard::Core::IKeyCodeMapper {
	public:
		std::optional<int> ToNative(Keyboard::Core::Enums::LogicalKey logicalKey) const override;
		std::optional<Keyboard::Core::Enums::LogicalKey> FromNative(int nativeVkCode) const override;
	};
}