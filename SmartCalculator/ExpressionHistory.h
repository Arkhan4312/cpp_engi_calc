#pragma once
#include <list>
#include <string>
#include <stdexcept>

class ExpressionHistory {
    std::list<std::string> expressions; // История введенных выражений
    std::list<double> results;           // Соответствующие результаты
    size_t max_size = 100;               // Максимальный размер истории

public:
    // Добавление новой записи в историю
    void add(const std::string& expression, double result);

    // Очистка истории
    void clear() noexcept;

    // Сохранение истории в файл
    void saveToFile(const std::string& filename) const;

    // Получение константных ссылок на данные
    const std::list<std::string>& getExpressions() const noexcept;
    const std::list<double>& getResults() const noexcept;

    // Управление размером истории
    void setMaxSize(size_t new_size) noexcept;
    size_t getMaxSize() const noexcept;
};