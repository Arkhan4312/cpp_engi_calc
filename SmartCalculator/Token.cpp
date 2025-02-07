#include "Token.h"
#include <string>
#include <cctype>


std::list<std::string> Token::AllExpressions;

//space clearing
std::string Token::removeSpaces(std::string expression) {
    size_t posEr = 0;
    while (posEr < expression.size()) {
        if (isspace(expression[posEr])) {
            expression.erase(posEr, 1); 
        }
        else {
            posEr++;
        }
    }
    return expression;
}

std::vector<Token> Token::Tokenize(const std::string& expression)
{
    std::vector<Token> tokens;
    size_t pos = 0;
    std::string cleanedExpression = removeSpaces(expression);
    AllExpressions.push_front(cleanedExpression);
    bool lastWasOperator = false;
    
    while (pos < cleanedExpression.size())
    {
        //Numbers find, also negative numbers  
        if (isdigit(cleanedExpression[pos]) || (cleanedExpression[pos] == '-' && (pos == 0 || cleanedExpression[pos - 1] == '(' || cleanedExpression[pos - 1] == '-')))
        {

            size_t start = pos;


            if (cleanedExpression[pos] == '-') {
                pos++;
            }


            while (pos < cleanedExpression.size() && (isdigit(cleanedExpression[pos]) || cleanedExpression[pos] == '.'))
            {
                pos++;
            }

            tokens.push_back(Token(TokenType::Number, cleanedExpression.substr(start, pos - start)));
            lastWasOperator = false;
        }
        //operators tokenize
        else if (cleanedExpression[pos] == '+' || cleanedExpression[pos] == '-' || cleanedExpression[pos] == '*' || cleanedExpression[pos] == '/' || cleanedExpression[pos] == '^')
        {
            if (pos == 0 && (cleanedExpression[pos] == '+' || cleanedExpression[pos] == '*' || cleanedExpression[pos] == '/' || cleanedExpression[pos] == '^')) {
                throw std::runtime_error("Syntax error: expression starts with an operator.");
            }

            if (lastWasOperator && cleanedExpression[pos] != '-') {
                throw std::runtime_error("Syntax error: two consecutive operators detected.");
            }

            tokens.push_back(Token(TokenType::Oper, std::string(1, cleanedExpression[pos])));
            pos++;
            lastWasOperator = true;
        }
        //parenthesis tokenize
        else if (cleanedExpression[pos] == '(' || cleanedExpression[pos] == ')')
        {
            tokens.push_back(Token(TokenType::Parenthesis, std::string(1, cleanedExpression[pos])));
            pos++;
            lastWasOperator = false;
        }

        //special symbols tokenize as numbers
        else if (cleanedExpression[pos] == 'p')
        {
            tokens.push_back(Token(TokenType::Number, "3.14159265358979323846"));
            pos++;
            if (cleanedExpression[pos] == 'i')
            {
                pos++;
                lastWasOperator = false;
            }
        }
        else if (cleanedExpression[pos] == 'e')
        {
            tokens.push_back(Token(TokenType::Number, "2.71828182845904523536"));
            pos++;
            lastWasOperator = false;
        }

        //function tokenize
        else if (cleanedExpression[pos] == 'l' && pos + 2 < cleanedExpression.size() && cleanedExpression[pos + 1] == 'n' && cleanedExpression[pos + 2] == '(')
        {
            tokens.push_back(Token(TokenType::Function, "ln"));
            pos += 3;
            lastWasOperator = false;
        }
        else if (cleanedExpression[pos] == 'l' && pos + 3 < cleanedExpression.size() && cleanedExpression[pos + 1] == 'o' && cleanedExpression[pos + 2] == 'g' && cleanedExpression[pos + 3] == '(')
        {
            tokens.push_back(Token(TokenType::Function, "log"));
            pos += 4;
            lastWasOperator = false;
        }
        else if (cleanedExpression[pos] == 's' && pos + 3 < cleanedExpression.size() && cleanedExpression[pos + 1] == 'i' && cleanedExpression[pos + 2] == 'n' && cleanedExpression[pos + 3] == '(')
        {
            tokens.push_back(Token(TokenType::Function, "sin"));
            pos += 4;
            lastWasOperator = false;
        }
        else if (cleanedExpression[pos] == 'c' && pos + 3 < cleanedExpression.size() && cleanedExpression[pos + 1] == 'o' && cleanedExpression[pos + 2] == 's' && cleanedExpression[pos + 3] == '(')
        {
            tokens.push_back(Token(TokenType::Function, "cos"));
            pos += 4;
            lastWasOperator = false;
        }
        else if (cleanedExpression[pos] == 't' && pos + 3 < cleanedExpression.size() && cleanedExpression[pos + 1] == 'a' && cleanedExpression[pos + 2] == 'n' && cleanedExpression[pos + 3] == '(')
        {
            tokens.push_back(Token(TokenType::Function, "tan"));
            pos += 4;
            lastWasOperator = false;
        }
        else if (cleanedExpression[pos] == 'c' && pos + 3 < cleanedExpression.size() && cleanedExpression[pos + 1] == 'o' && cleanedExpression[pos + 2] == 't' && cleanedExpression[pos + 3] == '(')
        {
            tokens.push_back(Token(TokenType::Function, "cot"));
            pos += 4;
            lastWasOperator = false;
        }
        else if (cleanedExpression[pos] == 's' && cleanedExpression[pos + 1] == 'q' && cleanedExpression[pos + 2] == 'r' && cleanedExpression[pos + 3] == 't' && cleanedExpression[pos + 4] == '(' && pos + 4 < cleanedExpression.size())
        {
            tokens.push_back(Token(TokenType::Function, "sqrt"));
            pos += 5;
        }


   
        //error throw
        else {
            throw std::runtime_error("Wrong symbol " + std::string(1, cleanedExpression[pos]));
            lastWasOperator = false;
        }

    }
    //end of expression
    tokens.push_back(Token(TokenType::End, ""));



    return tokens;
}
