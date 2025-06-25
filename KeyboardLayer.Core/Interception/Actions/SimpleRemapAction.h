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

			//	// ВАЖНО:
			//	// Если ты подменяешь клавишу, обязательно обрабатывай и нажатие (key down), и отпускание (key up).
			//	// Иначе система подумает, что клавиша осталась "зажатой", если ты отправил только нажатие или только отпускание.
			//	// См. remapped.state — он должен совпадать с оригиналом.
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