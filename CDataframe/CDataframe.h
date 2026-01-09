#pragma once

#include <vector>
#include <optional>

#include "../Column/Column.h"

class CDataframe
{
private:
    std::vector<Column> columns;

public:
    CDataframe();
    CDataframe(const std::vector<Column> &cols);
    CDataframe(std::initializer_list<Column> cols);
    ~CDataframe();

    void addCol(Column *col);
    void rmCol(int idx);
    void addRow(int idx);
    void rmRow(int idx);

    // Display
    void print(std::optional<int> firstRowOpt, std::optional<int> lastRowOpt, const std::vector<Column> *colOpt = nullptr);
    void display();
    void head(std::optional<int> row = std::nullopt);
    void tail(std::optional<int> row = std::nullopt);
    void byCol(const std::vector<Column>& col);

    // Helper
    
    int sizeBiggestCol();
};
