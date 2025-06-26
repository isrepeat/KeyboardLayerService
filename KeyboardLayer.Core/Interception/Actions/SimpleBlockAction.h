#pragma once
#include <Helpers/Logger.h>
#include "IKeyAction.h"

namespace Interception::Actions {
	class SimpleBlockAction : public IKeyAction {
	public:
		Enums::ActionResult Apply(InterceptionKeyStroke& keyStroke) const override {
			LOG_DEBUG_D("[Blocked] Key code: {}"
				, keyStroke.code
			);
			return Enums::ActionResult::Handled;
		}
	};
}