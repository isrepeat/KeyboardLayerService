#pragma once
#include "Keyboard/Core/IKeyCodeMapper.h"
#include "IKeyRemapper.h"
#include <memory>
#include <map>

namespace Interception {
	class SimpleKeyRemapper : public IKeyRemapper {
	public:
		SimpleKeyRemapper(
			std::wstring deviceIdMatch, 
			std::map<Keyboard::Core::Enums::LogicalKey, Keyboard::Core::Enums::LogicalKey> keyMap);

		bool ShouldRemap(const DeviceInfo& device, const InterceptionKeyStroke& stroke) const override;
		InterceptionKeyStroke Remap(const InterceptionKeyStroke& original) const override;

	private:
		std::wstring deviceIdMatch;
		std::map<Keyboard::Core::Enums::LogicalKey, Keyboard::Core::Enums::LogicalKey> keyMap;
		std::shared_ptr<Keyboard::Core::IKeyCodeMapper> keyCodeMapper;
	};
}