#pragma once
#include <Helpers/Logger.h>
#include "IKeyAction.h"

namespace Interception::Actions {
	class SimpleBlockAction : public IKeyAction {
	public:
		Result Apply(InterceptionKeyStroke& keyStroke) const override {
			LOG_DEBUG_D("[Blocked] Key code: {}"
				, keyStroke.code
			);
			return Result::Handled;
		}
	};
}