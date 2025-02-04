#include "Parser.h"
#include <cctype>
#include <algorithm>

using namespace std;

string Parser::removeSpaces(const string& expr) {
    string result;
    result.reserve(expr.size());
    for (char c : expr) {
        if (!isspace(c)) result += c;
    }
    return result;
}

void Parser::validateCurrentChar(char c, size_t pos) const {
    if (!isalnum(c) && !strchr("+-*/.^()", c)) {
        throw runtime_error("Invalid character '" + string(1, c) +
            "' at position " + to_string(pos));
    }
}

void Parser::parseNumber(const string& expr, size_t& pos, vector<Token>& tokens) {
    size_t start = pos;
    bool hasDecimal = false;

    if (expr[pos] == '-') pos++;

    while (pos < expr.size()) {
        if (isdigit(expr[pos])) {
            pos++;
        }
        else if (expr[pos] == '.' && !hasDecimal) {
            hasDecimal = true;
            pos++;
        }
        else break;
    }

    tokens.emplace_back(TokenType::Number, expr.substr(start, pos - start));
}

void Parser::parseFunctionOrVariable(const string& expr, size_t& pos, vector<Token>& tokens) {
    size_t start = pos;
    while (pos < expr.size() && (isalnum(expr[pos]) || expr[pos] == '_')) {
        pos++;
    }

    string ident = expr.substr(start, pos - start);

    // Проверка зарезервированных функций
    if (pos < expr.size() && expr[pos] == '(') {
        static const vector<string> FUNCTIONS = {
            "sin", "cos", "tan", "cot", "log", "ln", "sqrt"
        };

        if (find(FUNCTIONS.begin(), FUNCTIONS.end(), ident) != FUNCTIONS.end()) {
            tokens.emplace_back(TokenType::Function, ident);
            pos++; // Пропускаем '('
            return;
        }
    }

    // Проверка констант
    if (ident == "pi") {
        tokens.emplace_back(TokenType::Number, "3.14159265358979323846");
        return;
    }
    if (ident == "e") {
        tokens.emplace_back(TokenType::Number, "2.71828182845904523536");
        return;
    }

    // Если не функция и не константа - считаем переменной
    tokens.emplace_back(TokenType::Variable, ident);
}

void Parser::parseOperator(char c, size_t pos, vector<Token>& tokens, bool& lastWasOperator) {
    if (strchr("+-*/^", c)) {
        if (c == '-' && (pos == 0 || tokens.empty() ||
            tokens.back().type == TokenType::Oper ||
            tokens.back().type == TokenType::Parenthesis)) {
            tokens.emplace_back(TokenType::Number, "0");
        }
        tokens.emplace_back(TokenType::Oper, string(1, c));
        lastWasOperator = true;
    }
    else if (c == '(' || c == ')') {
        tokens.emplace_back(TokenType::Parenthesis, string(1, c));
        lastWasOperator = (c == '(');
    }
}

vector<Token> Parser::parse(const string& expression) {
    string cleanedExpr = removeSpaces(expression);
    m_history.push_front(cleanedExpr);

    vector<Token> tokens;
    size_t pos = 0;
    bool lastWasOperator = false;

    while (pos < cleanedExpr.size()) {
        char c = cleanedExpr[pos];
        validateCurrentChar(c, pos);

        if (isdigit(c) || (c == '-' && (pos == 0 || cleanedExpr[pos - 1] == '('))) {
            parseNumber(cleanedExpr, pos, tokens);
            lastWasOperator = false;
        }
        else if (isalpha(c) || c == '_') {
            parseFunctionOrVariable(cleanedExpr, pos, tokens);
            lastWasOperator = false;
        }
        else {
            parseOperator(c, pos, tokens, lastWasOperator);
            pos++;
        }
    }

    tokens.emplace_back(TokenType::End, "");
    return tokens;
}

const list<string>& Parser::getHistory() const noexcept {
    return m_history;
}