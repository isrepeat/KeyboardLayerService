#pragma once
#include <Helpers/Logger.h>
#include "Interception/InterceptionKeyCodeMapper.h"
#include "Keyboard/Platform/WindowsKeyCodeMapper.h"
#include "Keyboard/Platform/Win32KeySender.h"
#include "Keyboard/Platform/ModifierState.h"
#include "Keyboard/Core/LogicalKeyGroup.h"
#include "Keyboard/Core/LogicalKey.h"
#include "IKeyAction.h"

#include <unordered_map>
#include <memory>
#include <vector>

namespace Interception::Actions {
	class ExtendCapsLockFunctionalityAction : public IKeyAction {
	public:
		ExtendCapsLockFunctionalityAction()
			: win32KeySender{ std::make_unique<Keyboard::Platform::Win32KeySender>() }
			, windowsKeyCodeMapper{} 
			, interceptionKeyCodeMapper{}
			, digitsLogicalKKeyGroup{ Keyboard::Core::LogicalKeyGroup::Digits() }
			, digitsToFunctionalsMap{ 
				//{ Keyboard::Core::Enums::LogicalKey::Digit1, Keyboard::Core::Enums::LogicalKey::F1 },
				//{ Keyboard::Core::Enums::LogicalKey::Digit2, Keyboard::Core::Enums::LogicalKey::F2 },
				//{ Keyboard::Core::Enums::LogicalKey::Digit3, Keyboard::Core::Enums::LogicalKey::F3 },
				//{ Keyboard::Core::Enums::LogicalKey::Digit4, Keyboard::Core::Enums::LogicalKey::F4 },
				{ Keyboard::Core::Enums::LogicalKey::Digit5, Keyboard::Core::Enums::LogicalKey::F5 },
				//{ Keyboard::Core::Enums::LogicalKey::Digit6, Keyboard::Core::Enums::LogicalKey::F6 },
				//{ Keyboard::Core::Enums::LogicalKey::Digit7, Keyboard::Core::Enums::LogicalKey::F7 },
				//{ Keyboard::Core::Enums::LogicalKey::Digit8, Keyboard::Core::Enums::LogicalKey::F8 },
				{ Keyboard::Core::Enums::LogicalKey::Digit9, Keyboard::Core::Enums::LogicalKey::F9 },
				{ Keyboard::Core::Enums::LogicalKey::Digit0, Keyboard::Core::Enums::LogicalKey::F10 },
				{ Keyboard::Core::Enums::LogicalKey::Minus, Keyboard::Core::Enums::LogicalKey::F11 },
				//{ Keyboard::Core::Enums::LogicalKey::Plus, Keyboard::Core::Enums::LogicalKey::F12 }, 
			}
		{}

		Enums::ActionResult Apply(InterceptionKeyStroke& keyStroke) const override {
			bool isCapsLockActive = Keyboard::Platform::ModifierState::IsCapsLockActive();
			bool isShiftPressed = Keyboard::Platform::ModifierState::IsShiftPressed();

			if (isCapsLockActive) {
				auto logicalKey = this->interceptionKeyCodeMapper.FromNative(keyStroke.code);
				if (!logicalKey) {
					return Enums::ActionResult::NotHandled;
				}

				if (this->digitsLogicalKKeyGroup.Contains(*logicalKey)) {
					auto functionalLogicalKey = this->digitsToFunctionalsMap.at(*logicalKey);

					auto win32NativeKey = this->windowsKeyCodeMapper.ToNative(functionalLogicalKey);
					if (!win32NativeKey) {
						return Enums::ActionResult::NotHandled;
					}

					if (keyStroke.state == InterceptionKeyState::INTERCEPTION_KEY_DOWN) {
						this->win32KeySender->PressKey(*win32NativeKey);
					}

					if (keyStroke.state == InterceptionKeyState::INTERCEPTION_KEY_UP) {
						this->win32KeySender->ReleaseKey(*win32NativeKey);
					}

					return Enums::ActionResult::Handled;
				}
				else {
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
			}

			return Enums::ActionResult::NotHandled;
		}

	private:
		std::unique_ptr<Keyboard::Platform::Win32KeySender> win32KeySender;
		Keyboard::Platform::WindowsKeyCodeMapper windowsKeyCodeMapper;
		InterceptionKeyCodeMapper interceptionKeyCodeMapper;

		const Keyboard::Core::LogicalKeyGroup digitsLogicalKKeyGroup;
		const std::unordered_map<Keyboard::Core::Enums::LogicalKey, Keyboard::Core::Enums::LogicalKey> digitsToFunctionalsMap;
	};
}