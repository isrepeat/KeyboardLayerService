#pragma once
#include "3rdParty/Interception/interception.h"

namespace Interception::Actions {
	class IKeyAction {
	public:
		enum class Result {
			NotHandled,             // Не обработана (или можно передавать дальше)
			Handled,                // Клавиша обработана и поглощена (не передавать)
			HandledPassThrough,     // Клавиша обработана, вызываем следующий обработчик
		};

		virtual ~IKeyAction() = default;
		virtual Result Apply(InterceptionKeyStroke& keyStroke) const = 0;
	};
}