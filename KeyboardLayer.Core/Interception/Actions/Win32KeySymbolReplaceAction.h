#pragma once
#include <Helpers/Logger.h>
#include "Keyboard/Platform/WindowsKeyCodeMapper.h"
#include "Keyboard/Platform/Win32KeySender.h"
#include "Keyboard/Core/LogicalKey.h"
#include "IKeyAction.h"
#include <memory>
#include <vector>

namespace Interception::Actions {
	class Win32KeySymbolReplaceAction : public IKeyAction {
	public:
		Win32KeySymbolReplaceAction(wchar_t replacedSybmol)
			: replacedSybmol{ replacedSybmol }
			, win32KeySender{ std::make_unique<Keyboard::Platform::Win32KeySender>() }
			, windowsKeyCodeMapper{} 
		{}

		Enums::ActionResult Apply(InterceptionKeyStroke& keyStroke) const override {
			if (keyStroke.state == InterceptionKeyState::INTERCEPTION_KEY_DOWN) {
				this->win32KeySender->SendUnicodeCharKeyDown(this->replacedSybmol);
			}

			if (keyStroke.state == InterceptionKeyState::INTERCEPTION_KEY_UP) {
				this->win32KeySender->SendUnicodeCharKeyUp(this->replacedSybmol);
			}

			return Enums::ActionResult::Handled;
		}

	private:
		wchar_t replacedSybmol;
		std::unique_ptr<Keyboard::Platform::Win32KeySender> win32KeySender;
		Keyboard::Platform::WindowsKeyCodeMapper windowsKeyCodeMapper;
	};
}