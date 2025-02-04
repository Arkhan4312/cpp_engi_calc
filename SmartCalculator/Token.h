#pragma once
#include <string>
#include <list>

enum class TokenType {
    Number, Oper, Parenthesis, Function, Variable, End
};

struct Token {
    TokenType type;
    std::string value;

    Token() = default;
    Token(TokenType t, const std::string& v) : type(t), value(v) {}
};