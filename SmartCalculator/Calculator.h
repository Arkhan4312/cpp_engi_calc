#pragma once
#include "Token.h"
#include <stack>
class Calculator : public Token
{
public:
    Calculator();  // Конструктор по умолчанию
    ~Calculator() = default;
    int getPrecedence(char op);
    double applyOperator(double a, double b, char op);
    bool areParenthesesBalanced(const std::string& expr);
    double evaluation(const std::vector<Token>& tokens);
    void HandleFunctionCall(const std::string& funcName, double r, std::stack<double>& values);
    bool validateExpression(const std::vector<Token>& tokens, const std::string& expr);

};
