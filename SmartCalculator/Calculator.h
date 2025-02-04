#pragma once
#include "Parser.h"
#include <stack>
#include <cmath>
#include <stdexcept>
#include <unordered_map>

class Calculator {
    std::unordered_map<std::string, double> variables; // Хранилище переменных
    Parser parser; // Интеграция с парсером
    
    int getPrecedence(char op) const;
    double applyOperator(double a, double b, char op) const;
    void handleFunction(const std::string& funcName, double arg, std::stack<double>& values) const;
    void validateExpression(const std::vector<Token>& tokens) const;

public:
    explicit Calculator() = default;
    
    double evaluate(const std::string& expression);
    void setVariable(const std::string& name, double value);
};