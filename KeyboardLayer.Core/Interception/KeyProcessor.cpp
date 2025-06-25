#include "KeyProcessor.h"

namespace Interception {
	KeyProcessor::KeyProcessor(
		std::shared_ptr<Rules::IKeyRule> rule,
		std::shared_ptr<Actions::IKeyAction> action)
		: rule{ std::move(rule) }
		, action{ std::move(action) }
	{}

	Actions::IKeyAction::Result KeyProcessor::Process(const DeviceInfo& device, InterceptionKeyStroke& keyStroke) const {
		if (!this->rule->IsMatch(device, keyStroke)) {
			return Actions::IKeyAction::Result::NotHandled;
		}
		return this->action->Apply(keyStroke);
	}
}