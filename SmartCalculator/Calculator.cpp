#include "Calculator.h"
#include <vector>
#include <algorithm>

using namespace std;

int Calculator::getPrecedence(char op) const {
    static const unordered_map<char, int> precedence{
        {'+', 1}, {'-', 1},
        {'*', 2}, {'/', 2},
        {'^', 3}
    };
    return precedence.count(op) ? precedence.at(op) : 0;
}

double Calculator::applyOperator(double a, double b, char op) const {
    switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/':
        if (b == 0) throw runtime_error("Division by zero");
        return a / b;
    case '^': return pow(a, b);
    default: throw runtime_error("Unknown operator: " + string(1, op));
    }
}

void Calculator::handleFunction(const string& funcName, double arg, stack<double>& values) const {
    static const unordered_map<string, double(*)(double)> functions{
        {"sin",  sin},  {"cos",  cos}, {"tan", tan},
        {"cot", [](double x) {return 1 / tan(x);}},
        {"log", log10}, {"ln",   log},
        {"sqrt", sqrt}
    };

    if (functions.count(funcName)) {
        values.push(functions.at(funcName)(arg));
    }
    else {
        throw runtime_error("Unknown function: " + funcName);
    }
}

void Calculator::validateExpression(const vector<Token>& tokens) const {
    stack<char> parens;
    bool lastWasOper = true; // Разрешаем унарный минус в начале

    for (const auto& token : tokens) {
        switch (token.type) {
        case TokenType::Parenthesis:
            if (token.value == "(") {
                parens.push('(');
                lastWasOper = true;
            }
            else {
                if (parens.empty()) throw runtime_error("Mismatched parentheses");
                parens.pop();
                lastWasOper = false;
            }
            break;

        case TokenType::Oper:
            if (lastWasOper && token.value != "-")
                throw runtime_error("Duplicate operators");
            lastWasOper = (token.value != ")");
            break;

        default:
            lastWasOper = false;
        }
    }

    if (!parens.empty()) throw runtime_error("Mismatched parentheses");
}

void Calculator::setVariable(const string& name, double value) {
    variables[name] = value;
}

double Calculator::evaluate(const string& expression) {
    auto tokens = parser.parse(expression);
    validateExpression(tokens);

    stack<double> values;
    stack<char> operators;
    stack<string> functions;

    for (const auto& token : tokens) {
        switch (token.type) {
        case TokenType::Number:
            values.push(stod(token.value));
            break;

        case TokenType::Variable:
            if (variables.count(token.value)) {
                values.push(variables.at(token.value));
            }
            else {
                throw runtime_error("Undefined variable: " + token.value);
            }
            break;

        case TokenType::Function:
            functions.push(token.value);
            operators.push('(');
            break;

        case TokenType::Oper: {
            while (!operators.empty() && getPrecedence(operators.top()) >= getPrecedence(token.value[0])) {
                double b = values.top(); values.pop();
                double a = values.top(); values.pop();
                values.push(applyOperator(a, b, operators.top()));
                operators.pop();
            }
            operators.push(token.value[0]);
            break;
        }

        case TokenType::Parenthesis:
            if (token.value == "(") {
                operators.push('(');
            }
            else {
                while (operators.top() != '(') {
                    double b = values.top(); values.pop();
                    double a = values.top(); values.pop();
                    values.push(applyOperator(a, b, operators.top()));
                    operators.pop();
                }
                operators.pop();

                if (!functions.empty()) {
                    double arg = values.top(); values.pop();
                    handleFunction(functions.top(), arg, values);
                    functions.pop();
                }
            }
            break;

        case TokenType::End:
            while (!operators.empty()) {
                double b = values.top(); values.pop();
                double a = values.top(); values.pop();
                values.push(applyOperator(a, b, operators.top()));
                operators.pop();
            }
            break;
        }
    }

    if (values.size() != 1) {
        throw runtime_error("Malformed expression");
    }

    return values.top();
}