#pragma once
#include "Token.h"
#include <vector>

class Parser {
    std::list<std::string> m_history;  // История выражений
public:
    std::vector<Token> parse(const std::string& expr);
    const auto& history() const { return m_history; }
};