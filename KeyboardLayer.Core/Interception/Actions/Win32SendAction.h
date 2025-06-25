#pragma once
#include <Helpers/Logger.h>
#include "Keyboard/Platform/WindowsKeyCodeMapper.h"
#include "Keyboard/Platform/Win32KeySender.h"
#include "Keyboard/Core/LogicalKey.h"
#include "IKeyAction.h"
#include <memory>
#include <vector>

namespace Interception::Actions {
	class Win32SendAction : public IKeyAction {
	public:
		Win32SendAction(std::vector<Keyboard::Core::Enums::LogicalKey> keysToSend)
			: keysToSend{ std::move(keysToSend) }
			, win32KeySender{ std::make_unique<Keyboard::Platform::Win32KeySender>() }
			, windowsKeyCodeMapper{} 
		{}

		Result Apply(InterceptionKeyStroke& keyStroke) const override {
			for (auto logicalKey : this->keysToSend) {
				auto nativeKey = this->windowsKeyCodeMapper.ToNative(logicalKey);
				if (nativeKey) {
					this->win32KeySender->PressKey(static_cast<WORD>(*nativeKey));
				}
			}
			return Result::Handled;
		}

	private:
		std::vector<Keyboard::Core::Enums::LogicalKey> keysToSend;
		std::unique_ptr<Keyboard::Platform::Win32KeySender> win32KeySender;
		Keyboard::Platform::WindowsKeyCodeMapper windowsKeyCodeMapper;
	};



	class ExtendFunctionalityAction : public IKeyAction {
	public:
		ExtendFunctionalityAction()
			: win32KeySender{ std::make_unique<Keyboard::Platform::Win32KeySender>() }
			, windowsKeyCodeMapper{} {
		}

		Result Apply(InterceptionKeyStroke& keyStroke) const override {
			bool isCapsLockActive = (::GetKeyState(VK_CAPITAL) & 0x0001) != 0;
			bool isShiftPressed = (::GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;

			if (isCapsLockActive && !isShiftPressed) {
				if (keyStroke.state == InterceptionKeyState::INTERCEPTION_KEY_DOWN) {
					{
						INPUT input;
						input.type = INPUT_KEYBOARD;
						input.ki.wVk = VK_SHIFT;
						input.ki.dwFlags = 0;
						input.ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;
						SendInput(1, &input, sizeof(INPUT));
					}
					{
						INPUT input;
						input.type = INPUT_KEYBOARD;
						input.ki.wVk = 0x47;
						input.ki.dwFlags = 0;
						SendInput(1, &input, sizeof(INPUT));
					}
					{
						INPUT input;
						input.type = INPUT_KEYBOARD;
						input.ki.wVk = VK_SHIFT;
						input.ki.dwFlags = KEYEVENTF_KEYUP;
						input.ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;
						SendInput(1, &input, sizeof(INPUT));
					}
				}

				if (keyStroke.state == InterceptionKeyState::INTERCEPTION_KEY_UP) {
					{
						INPUT input;
						input.type = INPUT_KEYBOARD;
						input.ki.wVk = 0x47;
						input.ki.dwFlags = KEYEVENTF_KEYUP;
						SendInput(1, &input, sizeof(INPUT));
					}
				}

				return Result::Handled;
			}




			//for (auto logicalKey : this->keysToSend) {
			//	auto nativeKey = this->windowsKeyCodeMapper.ToNative(logicalKey);
			//	if (nativeKey) {
			//		this->win32KeySender->PressKey(static_cast<WORD>(*nativeKey));
			//	}
			//}
			//return false;
		}

	private:
		std::vector<Keyboard::Core::Enums::LogicalKey> keysToSend;
		std::unique_ptr<Keyboard::Platform::Win32KeySender> win32KeySender;
		Keyboard::Platform::WindowsKeyCodeMapper windowsKeyCodeMapper;
	};
}