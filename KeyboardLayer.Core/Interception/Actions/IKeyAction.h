#pragma once
#include "3rdParty/Interception/interception.h"

namespace Interception::Actions {
	class IKeyAction {
	public:
		enum class Result {
			NotHandled,             // �� ���������� (��� ����� ���������� ������)
			Handled,                // ������� ���������� � ��������� (�� ����������)
			HandledPassThrough,     // ������� ����������, �������� ��������� ����������
		};

		virtual ~IKeyAction() = default;
		virtual Result Apply(InterceptionKeyStroke& keyStroke) const = 0;
	};
}