#include "CSVLineReader.h"
#include <iostream>
#include <fstream>
#include <stdexcept>

// Конструктор
CSVLineReader::CSVLineReader(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл: " + filename);
    }

    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    file.close();
}

// Получение строки по индексу
std::string CSVLineReader::getLine(size_t index) const {
    if (index >= lines.size()) {
        throw std::out_of_range("Индекс строки выходит за пределы данных");
    }
    return lines[index];
}

// Получение количества строк
size_t CSVLineReader::lineCount() const {
    return lines.size();
}

// Вывод всех строк на экран
void CSVLineReader::printLines() const {
    for (const auto& line : lines) {
        std::cout << line << std::endl;
    }
}
