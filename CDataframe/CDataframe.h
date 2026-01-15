#pragma once

#include <vector>
#include <optional>
#include <memory>
#include <string>
#include <fstream>

#include "../Column/Column.h"

class CDataframe
{
private:
    std::vector<std::shared_ptr<Column>> columns;

    // ===== DISPLAY =====
    void print(std::optional<int> firstRowOpt, std::optional<int> lastRowOpt, const std::vector<Column> *colOpt = nullptr);

public:
    // Constructors
    CDataframe();
    CDataframe(const std::vector<ColumnType>& types);
    CDataframe(const std::vector<Column> &cols);
    CDataframe(std::initializer_list<Column> cols);
    ~CDataframe();

    
    // Display
    void display();
    void head(std::optional<int> row = std::nullopt);
    void tail(std::optional<int> row = std::nullopt);
    void displayCol(const std::vector<Column>& col);
    void printHeader() const;
    
    // Operation

    void setColumnNames(const std::vector<std::string>& names);
    bool insertColumns(const std::vector<Column*>& cols);
    bool insertColumn(Column* col);
    bool deleteColumn(const std::string& colName);

    bool insertRows(const std::vector<std::vector<ColumnValue>>& rows);
    bool insertRow(const std::vector<ColumnValue>& values);
    bool deleteRow(const int idx);
    bool renameCol(Column* col, const std::string& newName);

    bool exist(const int val);
    bool replaceValue(const Column& col, const int index, const int newVal);
    
    //Statistics & Info
    size_t getColumnsCount() const;
    size_t getRowsCount() const;
    int numberOfRows();
    int numberOfCols();
    int numberOfCellsEqualTo(int x);
    int numberOfCellsGreaterThan(int x);
    int numberOfCellsLowerThan(int x);
    void info() const;

    // CSV Methods
    static std::unique_ptr<CDataframe> loadFromCSV(
        const std::string& filename,
        const std::vector<ColumnType>& types
    );
    static std::unique_ptr<CDataframe> loadFromCSVAuto(const std::string& filename);
    void saveToCSV(const std::string& filename) const;
    
    // Helper
    int sizeBiggestCol();
    std::shared_ptr<Column> getColumnByName(const std::string& name);
    std::shared_ptr<Column> getColumnByIndex(size_t index);
};


