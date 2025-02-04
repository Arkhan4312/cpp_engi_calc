#include "ExpressionHistory.h"
#include <fstream>
#include <iomanip>
#include <algorithm>

void ExpressionHistory::add(const std::string& expression, double result) {
    // ѕроверка и ограничение размера истории
    if (expressions.size() >= max_size) {
        expressions.pop_back();
        results.pop_back();
    }

    expressions.push_front(expression);
    results.push_front(result);
}

void ExpressionHistory::clear() noexcept {
    expressions.clear();
    results.clear();
}

void ExpressionHistory::saveToFile(const std::string& filename) const {
    std::ofstream outFile(filename);
    if (!outFile) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    // «аголовок таблицы
    outFile << std::setw(5) << "No."
        << std::setw(25) << "Expression"
        << std::setw(20) << "Result"
        << "\n-------------------------------------------------\n";

    // »тераци€ по истории с использованием zip-итераторов
    auto exprIt = expressions.begin();
    auto resIt = results.begin();
    size_t index = 1;

    while (exprIt != expressions.end() && resIt != results.end()) {
        outFile << std::setw(5) << index++
            << std::setw(25) << *exprIt
            << std::setw(20) << std::setprecision(8) << *resIt
            << '\n';

        ++exprIt;
        ++resIt;
    }

    outFile.close();
}

const std::list<std::string>& ExpressionHistory::getExpressions() const noexcept {
    return expressions;
}

const std::list<double>& ExpressionHistory::getResults() const noexcept {
    return results;
}

void ExpressionHistory::setMaxSize(size_t new_size) noexcept {
    max_size = std::max(new_size, static_cast<size_t>(1));

    // ќбрезка истории при уменьшении размера
    while (expressions.size() > max_size) {
        expressions.pop_back();
        results.pop_back();
    }
}

size_t ExpressionHistory::getMaxSize() const noexcept {
    return max_size;
}