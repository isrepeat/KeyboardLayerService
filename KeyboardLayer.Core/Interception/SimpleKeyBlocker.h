#pragma once
#include "Keyboard/Core/IKeyCodeMapper.h"
#include "IKeyBlocker.h"
#include <memory>
#include <set>

namespace Interception {
	class SimpleKeyBlocker : public IKeyBlocker {
	public:
		SimpleKeyBlocker(
			std::wstring deviceIdMatch,
			std::set<Keyboard::Core::Enums::LogicalKey> logicalKeysToBlock);

		bool ShouldBlock(const DeviceInfo& device, const InterceptionKeyStroke& stroke) const override;

	private:
		std::wstring deviceIdMatch;
		std::set<Keyboard::Core::Enums::LogicalKey> keysToBlock;
		std::unique_ptr<Keyboard::Core::IKeyCodeMapper> keyCodeMapper;
	};
}