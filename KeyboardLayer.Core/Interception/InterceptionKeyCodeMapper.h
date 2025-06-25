#pragma once
#include "Keyboard/Core/IKeyCodeMapper.h"

namespace Interception {
	class InterceptionKeyCodeMapper : public Keyboard::Core::IKeyCodeMapper {
	public:
		std::optional<int> ToNative(Keyboard::Core::Enums::LogicalKey logicalKey) const override;
		std::optional<Keyboard::Core::Enums::LogicalKey> FromNative(int nativeCode) const override;
	};
}