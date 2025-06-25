#pragma once
#include <Helpers/Logger.h>
#include "Interception/InterceptionKeyCodeMapper.h"
#include "Keyboard/Core/IKeyCodeMapper.h"
#include "IKeyAction.h"
#include <memory>
#include <map>

namespace Interception::Actions {
	class SimpleRemapAction : public IKeyAction {
	public:
		SimpleRemapAction(
			std::map<Keyboard::Core::Enums::LogicalKey, Keyboard::Core::Enums::LogicalKey> keyRemapTable)
			: keyRemapTable{ std::move(keyRemapTable) }
			, interceptionKeyCodeMapper{} 
		{}

		Result Apply(InterceptionKeyStroke& keyStroke) const override {
			auto logicalKey = this->interceptionKeyCodeMapper.FromNative(keyStroke.code);
			if (!logicalKey || !this->keyRemapTable.contains(*logicalKey)) {
				return Result::NotHandled;
			}

			auto to = this->keyRemapTable.at(*logicalKey);
			auto nativeKey = this->interceptionKeyCodeMapper.ToNative(to);
			if (nativeKey) {
				keyStroke.code = *nativeKey;
			}
			return Result::NotHandled;

			//	// �����:
			//	// ���� �� ���������� �������, ����������� ����������� � ������� (key down), � ���������� (key up).
			//	// ����� ������� ��������, ��� ������� �������� "�������", ���� �� �������� ������ ������� ��� ������ ����������.
			//	// ��. remapped.state � �� ������ ��������� � ����������.
			//	interception_send(this->context, device, (const InterceptionStroke*)&remapped, 1);

			//	LOG_DEBUG_D("[Remapped] Key code: {}  => {} on device {}"
			//		, key.code
			//		, remapped.code
			//		, device
			//	);
		}

	private:
		std::map<Keyboard::Core::Enums::LogicalKey, Keyboard::Core::Enums::LogicalKey> keyRemapTable;
		InterceptionKeyCodeMapper interceptionKeyCodeMapper;
	};
}