#pragma once
#include "Token.h"
#include <vector>
#include <list>
#include <stdexcept>

class Parser {
    std::list<std::string> m_history; // История всех обработанных выражений

    std::string removeSpaces(const std::string& expr);
    void validateCurrentChar(char c, size_t pos) const;
    void parseNumber(const std::string& expr, size_t& pos, std::vector<Token>& tokens);
    void parseFunctionOrVariable(const std::string& expr, size_t& pos, std::vector<Token>& tokens);
    void parseOperator(char c, size_t pos, std::vector<Token>& tokens, bool& lastWasOperator);

public:
    std::vector<Token> parse(const std::string& expression);
    const std::list<std::string>& getHistory() const noexcept;
};