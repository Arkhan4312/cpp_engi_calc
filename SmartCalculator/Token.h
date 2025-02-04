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

struct Token {
    TokenType type;
    std::string value;
    size_t position;  // Для отслеживания позиции в строке
};