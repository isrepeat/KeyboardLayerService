#pragma once
#include "3rdParty/Interception/interception.h"

// TODO: move to Helpers
namespace Enums {
	enum class ActionResult {
		NotHandled,             // Ќе обработана (или можно передавать дальше)
		Handled,                //  лавиша обработана и поглощена (не передавать)
	};
}

namespace Interception::Actions {
	class IKeyAction {
	public:
		virtual ~IKeyAction() = default;
		virtual Enums::ActionResult Apply(InterceptionKeyStroke& keyStroke) const = 0;
	};
}