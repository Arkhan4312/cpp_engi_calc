#include "Calculator.h"
#include <stack>
#include <cmath>
#include <stdexcept>
#include <unordered_map>

Calculator::Calculator() {}

// Precedence
int Calculator::getPrecedence(char op)
{
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;
}

// Simple orerators
double Calculator::applyOperator(double l, double r, char op)
{
    switch (op) {
    case '+': return l + r;
    case '-': return l - r;
    case '*': return l * r;
    case '/': 
        if (r == 0)
        {
            throw std::runtime_error("Division by zero");
        }
        return l / r;
    case '^': return pow(l, r);  

    default: throw std::runtime_error("Unknown operator: " + std::string(1, op));
    }
}

//Check balace of "(...)"
bool Calculator::areParenthesesBalanced(const std::string& expr) {
    std::stack<char> parentheses;   
    for (char ch : expr) {
        if (ch == '(') {
            parentheses.push(ch);
        }
        else if (ch == ')') {
            if (parentheses.empty()) {
                return false; 
            }
            parentheses.pop();
        }
    }
    return parentheses.empty(); 
}


// Correct input check
bool Calculator::validateExpression(const std::vector<Token>& tokens, const std::string& expr) {
    if (!areParenthesesBalanced(expr)) {
        throw std::runtime_error("Unbalanced parentheses. ");
    }
    return true; 
}

// Funcs
void Calculator::HandleFunctionCall(const std::string& funcName, double r, std::stack<double>& values)
{
    if (funcName == "log") {
        values.push(log10(r)); 
    }
    else if (funcName == "ln") {
        values.push(log(r)); 
    }
    else if (funcName == "sin") {
        values.push(sin(r)); 
    }
    else if (funcName == "cos") {
        values.push(cos(r)); 
    }
    else if (funcName == "tan") {
        values.push(tan(r)); 
    }
    else if (funcName == "cot") {
        values.push(1.0 / tan(r));  
    }
    else if (funcName == "sqrt")
    {
        values.push(sqrt(r));
    }
    else {
        throw std::runtime_error("Unknown function: " + funcName);
    }
}

// All evaluations block
double Calculator::evaluation(const std::vector<Token>& tokens) {
    std::stack<double> values;  
    std::stack<char> operators;
    std::stack<std::string> functionNames;

    size_t pos = 0;
    while (pos < tokens.size()) {
        const Token& token = tokens[pos];

        switch (token.type) {
            //numbers
        case TokenType::Number: {
         
            char* endPtr;
            double value = strtod(token.value.c_str(), &endPtr);
            if (*endPtr != '\0') {
                throw std::runtime_error("Invalid number format: " + token.value);
            }
            values.push(value);
            break;
        }
            //functions
        case TokenType::Function:
            functionNames.push(token.value); 
            operators.push('(');  
            break;

        case TokenType::Oper: {
            // operators
            while (!operators.empty() && getPrecedence(operators.top()) >= getPrecedence(token.value[0])) {
                double right = values.top(); values.pop();
                double left = values.top(); values.pop();
                char op = operators.top(); operators.pop();
                values.push(applyOperator(left, right, op));
            }
            operators.push(token.value[0]); 
            break;
        }



            //parenthesis
        case TokenType::Parenthesis:
            if (token.value == "(") {
                operators.push('('); 
            }
            else if (token.value == ")")
            {
                while (!operators.empty() && operators.top() != '(') {
                    double right = values.top(); values.pop();
                    double left = values.top(); values.pop();
                    char op = operators.top(); operators.pop();
                    values.push(applyOperator(left, right, op));
                }
                operators.pop(); 

               //argument check and calculate funcs;
                if (!functionNames.empty()) {
                    std::string funcName = functionNames.top();
                    functionNames.pop();
                    if (!values.empty()) {
                        double arg = values.top();
                        values.pop();
                        HandleFunctionCall(funcName, arg, values);
                    }
                    else {
                        throw std::runtime_error("Expected argument for function " + funcName);
                    }
                }
            }
            break;
        }

        ++pos; 
    }

   //other
    while (!operators.empty()) {
        double right = values.top(); values.pop();
        double left = values.top(); values.pop();
        char op = operators.top(); operators.pop();
        values.push(applyOperator(left, right, op));
    }

    // Actual results
    if (!values.empty()) {
        return values.top();
    }
    else {
        throw std::runtime_error("No result in evaluation.");
    }
}