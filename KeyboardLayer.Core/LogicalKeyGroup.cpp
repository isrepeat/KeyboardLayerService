#include "LogicalKeyGroup.h"

LogicalKeyGroup::LogicalKeyGroup(std::initializer_list<LogicalKey> keys)
    : keys(keys) {}

LogicalKeyGroup LogicalKeyGroup::operator|(const LogicalKeyGroup& other) const {
    LogicalKeyGroup result = *this;
    result.keys.insert(other.keys.begin(), other.keys.end());
    return result;
}

const std::set<LogicalKey>& LogicalKeyGroup::GetKeys() const {
    return this->keys;
}

bool LogicalKeyGroup::Contains(LogicalKey key) const {
    return this->keys.contains(key);
}

LogicalKeyGroup LogicalKeyGroup::Arrows() {
	return {
		LogicalKey::ArrowUp, LogicalKey::ArrowDown, LogicalKey::ArrowLeft, LogicalKey::ArrowRight,
		LogicalKey::Home, LogicalKey::End, LogicalKey::PageUp, LogicalKey::PageDown,
	};
}

LogicalKeyGroup LogicalKeyGroup::Digits() {
    return {
        LogicalKey::Digit0, LogicalKey::Digit1, LogicalKey::Digit2, LogicalKey::Digit3, LogicalKey::Digit4,
        LogicalKey::Digit5, LogicalKey::Digit6, LogicalKey::Digit7, LogicalKey::Digit8, LogicalKey::Digit9,
		LogicalKey::Minus, 
		LogicalKey::Plus,
    };
}

LogicalKeyGroup LogicalKeyGroup::Numpad() {
	return {
		LogicalKey::NumLock,
		LogicalKey::NumpadPlus,
		LogicalKey::NumpadMinus,
		LogicalKey::NumpadAsterisk,
		LogicalKey::NumpadSlash,
		LogicalKey::Numpad1,
		LogicalKey::Numpad2,
		LogicalKey::Numpad3,
		LogicalKey::Numpad4,
		LogicalKey::Numpad5,
		LogicalKey::Numpad6,
		LogicalKey::Numpad7,
		LogicalKey::Numpad8,
		LogicalKey::Numpad9,
		LogicalKey::NumpadEnter,
		LogicalKey::NumpadDot,
	};
}

LogicalKeyGroup LogicalKeyGroup::Letters() {
    return {
        LogicalKey::A, LogicalKey::B, LogicalKey::C, LogicalKey::D, LogicalKey::E, LogicalKey::F,
        LogicalKey::G, LogicalKey::H, LogicalKey::I, LogicalKey::J, LogicalKey::K, LogicalKey::L,
        LogicalKey::M, LogicalKey::N, LogicalKey::O, LogicalKey::P, LogicalKey::Q, LogicalKey::R,
        LogicalKey::S, LogicalKey::T, LogicalKey::U, LogicalKey::V, LogicalKey::W, LogicalKey::X,
        LogicalKey::Y, LogicalKey::Z, 
		LogicalKey::LeftBracket,
		LogicalKey::RightBracket,
		LogicalKey::Backslash,
		LogicalKey::Slash,
		LogicalKey::Semicolon,
		LogicalKey::Apostrophe,
		LogicalKey::Comma,
		LogicalKey::Period,
		LogicalKey::Tilde,
    };
}

LogicalKeyGroup LogicalKeyGroup::Modifiers() {
	return {
		LogicalKey::LeftShift, LogicalKey::RightShift,
		LogicalKey::LeftCtrl, LogicalKey::RightCtrl,
		LogicalKey::LeftAlt, LogicalKey::RightAlt,
		LogicalKey::LeftWin, LogicalKey::RightWin
	};
}

LogicalKeyGroup LogicalKeyGroup::ControlKeys() {
	return {
		LogicalKey::Enter,
		LogicalKey::Escape,
		LogicalKey::Backspace,
		LogicalKey::Tab,
		LogicalKey::Space,
		LogicalKey::CapsLock,
		LogicalKey::Insert,
		LogicalKey::Delete,
	};
}

LogicalKeyGroup LogicalKeyGroup::FunctionKeys() {
    return {
        LogicalKey::F1, LogicalKey::F2, LogicalKey::F3, LogicalKey::F4, LogicalKey::F5, LogicalKey::F6,
        LogicalKey::F7, LogicalKey::F8, LogicalKey::F9, LogicalKey::F10, LogicalKey::F11, LogicalKey::F12
    };
}

