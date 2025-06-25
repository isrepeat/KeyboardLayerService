#pragma once
#include "Interception/Actions/IKeyAction.h"
#include "Interception/Rules/IKeyRule.h"
#include <memory>

namespace Interception {
	class KeyProcessor {
	public:
		KeyProcessor(
			std::shared_ptr<Rules::IKeyRule> rule,
			std::shared_ptr<Actions::IKeyAction> action);

		Actions::IKeyAction::Result Process(const DeviceInfo& device, InterceptionKeyStroke& keyStroke) const;

	private:
		std::shared_ptr<Rules::IKeyRule> rule;
		std::shared_ptr<Actions::IKeyAction> action;
	};
}