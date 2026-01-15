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

    // Display
    void print(std::optional<int> firstRowOpt, std::optional<int> lastRowOpt, const std::vector<Column> *colOpt = nullptr);
    void display();
    void head(std::optional<int> row = std::nullopt);
    void tail(std::optional<int> row = std::nullopt);
    void displayCol(const std::vector<Column>& col);

    // Operation
    bool addCol(Column *col);
    bool rmCol(Column *col);
    bool addRow(const std::vector<int>& row);
    bool rmRow(const int idx);
    bool renameCol(Column* col, const std::string& newName);
    bool exist(const int val);
    bool replaceValue(const Column& col, const int index, const int newVal);

    //Statistics
    int numberOfRows();
    int numberOfCols();
    int numberOfCellsEqualTo(int x);
    int numberOfCellsGreaterThan(int x);
    int numberOfCellsLowerThan(int x);
    
    // Helper
    int sizeBiggestCol();
};
