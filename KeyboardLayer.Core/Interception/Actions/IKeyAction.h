#pragma once
#include "3rdParty/Interception/interception.h"

// TODO: move to Helpers
namespace Enums {
	enum class ActionResult {
		NotHandled,             // �� ���������� (��� ����� ���������� ������)
		Handled,                // ������� ���������� � ��������� (�� ����������)
	};
}

namespace Interception::Actions {
	class IKeyAction {
	public:
		virtual ~IKeyAction() = default;
		virtual Enums::ActionResult Apply(InterceptionKeyStroke& keyStroke) const = 0;
	};
}