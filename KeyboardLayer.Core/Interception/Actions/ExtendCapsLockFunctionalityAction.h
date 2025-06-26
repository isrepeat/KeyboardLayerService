#pragma once
#include <Helpers/Logger.h>
#include "Interception/InterceptionKeyCodeMapper.h"
#include "Keyboard/Platform/WindowsKeyCodeMapper.h"
#include "Keyboard/Platform/Win32KeySender.h"
#include "Keyboard/Platform/ModifierState.h"
#include "Keyboard/Core/LogicalKey.h"
#include "IKeyAction.h"
#include <memory>
#include <vector>

namespace Interception::Actions {
	class ExtendCapsLockFunctionalityAction : public IKeyAction {
	public:
		ExtendCapsLockFunctionalityAction()
			: win32KeySender{ std::make_unique<Keyboard::Platform::Win32KeySender>() }
			, windowsKeyCodeMapper{} 
			, interceptionKeyCodeMapper{}
		{}

		Enums::ActionResult Apply(InterceptionKeyStroke& keyStroke) const override {
			bool isCapsLockActive = Keyboard::Platform::ModifierState::IsCapsLockActive();
			bool isShiftPressed = Keyboard::Platform::ModifierState::IsShiftPressed();

			if (isCapsLockActive) {
				auto logicalKey = this->interceptionKeyCodeMapper.FromNative(keyStroke.code);
				if (!logicalKey) {
					return Enums::ActionResult::NotHandled;
				}

				auto win32NativeKey = this->windowsKeyCodeMapper.ToNative(*logicalKey);
				if (!win32NativeKey) {
					return Enums::ActionResult::NotHandled;
				}

				if (keyStroke.state == InterceptionKeyState::INTERCEPTION_KEY_DOWN) {
					// Swap shift state
					if (isShiftPressed) {
						this->win32KeySender->ReleaseKey(VK_SHIFT); // to emulate Capital letters
					}
					else {
						this->win32KeySender->PressKey(VK_SHIFT); // to emulate Lowercase letters
					}

					this->win32KeySender->PressKey(*win32NativeKey);

					if (isShiftPressed) {
						this->win32KeySender->PressKey(VK_SHIFT);
					}
					else {
						this->win32KeySender->ReleaseKey(VK_SHIFT);
					}
				}

				if (keyStroke.state == InterceptionKeyState::INTERCEPTION_KEY_UP) {
					this->win32KeySender->ReleaseKey(*win32NativeKey);
				}

				return Enums::ActionResult::Handled;
			}

			return Enums::ActionResult::NotHandled;
		}

	private:
		std::unique_ptr<Keyboard::Platform::Win32KeySender> win32KeySender;
		Keyboard::Platform::WindowsKeyCodeMapper windowsKeyCodeMapper;
		InterceptionKeyCodeMapper interceptionKeyCodeMapper;
	};
}