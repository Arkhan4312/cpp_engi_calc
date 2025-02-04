// Calculator.h
#pragma once
#include "Parser.h"
#include "ExpressionHistory.h"

class Calculator {
    Parser parser;
    ExpressionHistory history;

    double evaluateImpl(const std::vector<Token>& tokens);
public:
    double evaluate(const std::string& expr);
    const ExpressionHistory& getHistory() const;
};