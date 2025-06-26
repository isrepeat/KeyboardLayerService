#pragma once
#include "Interception/Actions/IKeyAction.h"
#include "Interception/Rules/IKeyRule.h"
#include <memory>

namespace Interception {
	class KeyProcessor {
	public:
		enum class ChainPolicy {
			StopOnHandled,     // ����������, ���� ����� Handled
			AlwaysContinue     // ������ ����������, ���������� �� ����������
		};

		KeyProcessor(
			std::shared_ptr<Rules::IKeyRule> rule,
			std::shared_ptr<Actions::IKeyAction> action,
			ChainPolicy chainPolicy);

		Enums::ActionResult Process(const DeviceInfo& device, InterceptionKeyStroke& keyStroke) const;
		ChainPolicy GetChainPolicy() const;

	private:
		std::shared_ptr<Rules::IKeyRule> rule;
		std::shared_ptr<Actions::IKeyAction> action;
		ChainPolicy chainPolicy;
	};
}