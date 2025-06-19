#pragma once
#include "LogicalKey.h"
#include <set>

class LogicalKeyGroup {
public:
    LogicalKeyGroup() = default;
    LogicalKeyGroup(std::initializer_list<LogicalKey> keys);

    LogicalKeyGroup operator|(const LogicalKeyGroup& other) const;
    const std::set<LogicalKey>& GetKeys() const;
    bool Contains(LogicalKey key) const;

    static LogicalKeyGroup Arrows();
    static LogicalKeyGroup Digits();
    static LogicalKeyGroup Numpad();
    static LogicalKeyGroup Letters();
	static LogicalKeyGroup Modifiers();
	static LogicalKeyGroup ControlKeys();
    static LogicalKeyGroup FunctionKeys();

private:
    std::set<LogicalKey> keys;
};