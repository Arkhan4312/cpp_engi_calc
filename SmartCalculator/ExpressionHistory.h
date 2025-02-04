#pragma once
#include <list>
#include <string>
#include <stdexcept>

class ExpressionHistory {
    std::list<std::string> expressions; // ������� ��������� ���������
    std::list<double> results;           // ��������������� ����������
    size_t max_size = 100;               // ������������ ������ �������

public:
    // ���������� ����� ������ � �������
    void add(const std::string& expression, double result);

    // ������� �������
    void clear() noexcept;

    // ���������� ������� � ����
    void saveToFile(const std::string& filename) const;

    // ��������� ����������� ������ �� ������
    const std::list<std::string>& getExpressions() const noexcept;
    const std::list<double>& getResults() const noexcept;

    // ���������� �������� �������
    void setMaxSize(size_t new_size) noexcept;
    size_t getMaxSize() const noexcept;
};