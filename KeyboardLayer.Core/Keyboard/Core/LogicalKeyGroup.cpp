#include "LogicalKeyGroup.h"

namespace Keyboard::Core {
	LogicalKeyGroup::LogicalKeyGroup(std::initializer_list<Enums::LogicalKey> keys)
		: keys{ keys }
	{}

	const std::set<Enums::LogicalKey>& LogicalKeyGroup::GetKeys() const {
		return this->keys;
	}

	bool LogicalKeyGroup::Contains(Enums::LogicalKey key) const {
		return this->keys.contains(key);
	}

	LogicalKeyGroup LogicalKeyGroup::operator|(const LogicalKeyGroup& other) const {
		LogicalKeyGroup result = *this;
		result.keys.insert(other.keys.begin(), other.keys.end());
		return result;
	}

	LogicalKeyGroup LogicalKeyGroup::operator+(const Enums::LogicalKey& logicalKey) const {
		LogicalKeyGroup result = *this;
		result.keys.insert(logicalKey);
		return result;
	}

	LogicalKeyGroup LogicalKeyGroup::operator-(const Enums::LogicalKey& logicalKey) const {
		LogicalKeyGroup result = *this;
		result.keys.erase(logicalKey);
		return result;
	}

	LogicalKeyGroup& LogicalKeyGroup::operator+=(const Enums::LogicalKey& logicalKey) {
		this->keys.insert(logicalKey);
		return *this;
	}

	LogicalKeyGroup& LogicalKeyGroup::operator-=(const Enums::LogicalKey& logicalKey) {
		this->keys.erase(logicalKey);
		return *this;
	}

	LogicalKeyGroup::operator std::set<Enums::LogicalKey>() const {
		return this->keys;
	}

	LogicalKeyGroup LogicalKeyGroup::Arrows() {
		return {
			Enums::LogicalKey::ArrowUp, Enums::LogicalKey::ArrowDown, Enums::LogicalKey::ArrowLeft, Enums::LogicalKey::ArrowRight,
			Enums::LogicalKey::Home, Enums::LogicalKey::End, Enums::LogicalKey::PageUp, Enums::LogicalKey::PageDown,
		};
	}

	LogicalKeyGroup LogicalKeyGroup::Digits() {
		return {
			Enums::LogicalKey::Digit0, Enums::LogicalKey::Digit1, Enums::LogicalKey::Digit2, Enums::LogicalKey::Digit3, Enums::LogicalKey::Digit4,
			Enums::LogicalKey::Digit5, Enums::LogicalKey::Digit6, Enums::LogicalKey::Digit7, Enums::LogicalKey::Digit8, Enums::LogicalKey::Digit9,
			Enums::LogicalKey::Minus,
			Enums::LogicalKey::Plus,
		};
	}

	LogicalKeyGroup LogicalKeyGroup::Numpad() {
		return {
			Enums::LogicalKey::NumLock,
			Enums::LogicalKey::NumpadPlus,
			Enums::LogicalKey::NumpadMinus,
			Enums::LogicalKey::NumpadAsterisk,
			Enums::LogicalKey::NumpadSlash,
			Enums::LogicalKey::Numpad1,
			Enums::LogicalKey::Numpad2,
			Enums::LogicalKey::Numpad3,
			Enums::LogicalKey::Numpad4,
			Enums::LogicalKey::Numpad5,
			Enums::LogicalKey::Numpad6,
			Enums::LogicalKey::Numpad7,
			Enums::LogicalKey::Numpad8,
			Enums::LogicalKey::Numpad9,
			Enums::LogicalKey::NumpadEnter,
			Enums::LogicalKey::NumpadDot,
		};
	}

	LogicalKeyGroup LogicalKeyGroup::Letters() {
		return {
			Enums::LogicalKey::A, Enums::LogicalKey::B, Enums::LogicalKey::C, Enums::LogicalKey::D, Enums::LogicalKey::E, Enums::LogicalKey::F,
			Enums::LogicalKey::G, Enums::LogicalKey::H, Enums::LogicalKey::I, Enums::LogicalKey::J, Enums::LogicalKey::K, Enums::LogicalKey::L,
			Enums::LogicalKey::M, Enums::LogicalKey::N, Enums::LogicalKey::O, Enums::LogicalKey::P, Enums::LogicalKey::Q, Enums::LogicalKey::R,
			Enums::LogicalKey::S, Enums::LogicalKey::T, Enums::LogicalKey::U, Enums::LogicalKey::V, Enums::LogicalKey::W, Enums::LogicalKey::X,
			Enums::LogicalKey::Y, Enums::LogicalKey::Z,
			Enums::LogicalKey::LeftBracket,
			Enums::LogicalKey::RightBracket,
			Enums::LogicalKey::Backslash,
			Enums::LogicalKey::Slash,
			Enums::LogicalKey::Semicolon,
			Enums::LogicalKey::Apostrophe,
			Enums::LogicalKey::Comma,
			Enums::LogicalKey::Period,
			Enums::LogicalKey::Tilde,
		};
	}

	LogicalKeyGroup LogicalKeyGroup::Modifiers() {
		return {
			Enums::LogicalKey::LeftShift, Enums::LogicalKey::RightShift,
			Enums::LogicalKey::LeftCtrl, Enums::LogicalKey::RightCtrl,
			Enums::LogicalKey::LeftAlt, Enums::LogicalKey::RightAlt,
			Enums::LogicalKey::LeftWin, Enums::LogicalKey::RightWin
		};
	}

	LogicalKeyGroup LogicalKeyGroup::ControlKeys() {
		return {
			Enums::LogicalKey::Enter,
			Enums::LogicalKey::Escape,
			Enums::LogicalKey::Backspace,
			Enums::LogicalKey::Tab,
			Enums::LogicalKey::Space,
			Enums::LogicalKey::CapsLock,
			Enums::LogicalKey::Insert,
			Enums::LogicalKey::Delete,
		};
	}

	LogicalKeyGroup LogicalKeyGroup::FunctionKeys() {
		return {
			Enums::LogicalKey::F1, Enums::LogicalKey::F2, Enums::LogicalKey::F3, Enums::LogicalKey::F4, Enums::LogicalKey::F5, Enums::LogicalKey::F6,
			Enums::LogicalKey::F7, Enums::LogicalKey::F8, Enums::LogicalKey::F9, Enums::LogicalKey::F10, Enums::LogicalKey::F11, Enums::LogicalKey::F12
		};
	}
}