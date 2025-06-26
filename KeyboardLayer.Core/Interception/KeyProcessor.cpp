#include "KeyProcessor.h"

namespace Interception {
	KeyProcessor::KeyProcessor(
		std::shared_ptr<Rules::IKeyRule> rule,
		std::shared_ptr<Actions::IKeyAction> action,
		ChainPolicy chainPolicy)
		: rule{ std::move(rule) }
		, action{ std::move(action) }
		, chainPolicy{ chainPolicy }
	{}

	Enums::ActionResult KeyProcessor::Process(const DeviceInfo& device, InterceptionKeyStroke& keyStroke) const {
		if (!this->rule->IsMatch(device, keyStroke)) {
			return Enums::ActionResult::NotHandled;
		}
		return this->action->Apply(keyStroke);
	}

	KeyProcessor::ChainPolicy KeyProcessor::GetChainPolicy() const {
		return this->chainPolicy;
	}
}