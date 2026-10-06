#pragma once
#ifndef CSV_LINE_READER_H
#define CSV_LINE_READER_H

#include <string>
#include <vector>

class CSVLineReader {
private:
    std::vector<std::string> lines; // Сохраненные строки из файла

public:
    // Конструктор
    explicit CSVLineReader(const std::string& filename);

    // Получение строки по индексу
    std::string getLine(size_t index) const;

    // Получение количества строк
    size_t lineCount() const;

    // Вывод всех строк на экран
    void printLines() const;
};

#endif // CSV_LINE_READER_H

