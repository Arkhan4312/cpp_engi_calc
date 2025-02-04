#pragma once
#include <iostream>
#include <vector>
#include <list>

enum class TokenType
{
    Number,
    Oper,
    Parenthesis,
    Function,
    Argument,
    Variable,
    Min,
    Max,
    Step,
    End
};

struct Token
{
    TokenType type;
    std::string value;

    Token() : type(TokenType::End), value("") {}

    Token(TokenType t, const std::string& v) : type(t), value(v) {}

    std::vector<Token> Tokenize(const std::string& expression);
    ~Token() = default;
    std::string removeSpaces(std::string expression);
    static std::list <std::string> AllExpressions;
};
