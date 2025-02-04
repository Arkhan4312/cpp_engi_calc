#pragma once
#include <list>
#include <string>

class ExpressionHistory {
    std::list<std::string> expressions;
    std::list<double> results;
public:
    void add(const std::string& expr, double result);
    void clear();
    void saveToFile(const std::string& filename) const;
};