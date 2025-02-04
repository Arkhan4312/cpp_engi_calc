#pragma once
#include <iostream>
#include "ExpressionHistory.h"

class UI {
    ExpressionHistory history;  // �������� ResTable � Token::AllExpressions
    int precision = 5;

    void ClearConsole();
    void NewExpression();
    bool ViewTable();
    void CreateTXT();
    bool Settings();

public:
    bool Menu();
};