#pragma once
#include "LogicalKey.h"
#include <set>

namespace Keyboard::Core {
	class LogicalKeyGroup {
	public:
		LogicalKeyGroup() = default;
		LogicalKeyGroup(std::initializer_list<Enums::LogicalKey> keys);

		LogicalKeyGroup operator|(const LogicalKeyGroup& other) const;
		const std::set<Enums::LogicalKey>& GetKeys() const;
		bool Contains(Enums::LogicalKey key) const;

		operator std::set<Enums::LogicalKey>() const;

		static LogicalKeyGroup Arrows();
		static LogicalKeyGroup Digits();
		static LogicalKeyGroup Numpad();
		static LogicalKeyGroup Letters();
		static LogicalKeyGroup Modifiers();
		static LogicalKeyGroup ControlKeys();
		static LogicalKeyGroup FunctionKeys();

	private:
		std::set<Enums::LogicalKey> keys;
	};
}