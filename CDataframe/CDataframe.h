#pragma once

#include <vector>
#include <optional>
#include <memory>
#include <string>
#include <fstream>

#include "../Column/Column.h"
#include "../Column/ColumnValue.h"

class CDataframe
{
private:
    std::vector<std::shared_ptr<Column>> columns;

public:
    // Constructors
    CDataframe();
    CDataframe(const std::vector<ColumnType>& types);
    CDataframe(const std::vector<Column> &cols);
    CDataframe(std::initializer_list<Column> cols);
    ~CDataframe();

    // Partie 8 - Core Methods
    void deleteColumn(const std::string& colName);
    void setColumnNames(const std::vector<std::string>& names);
    size_t getColumnsCount() const;
    bool insertRow(const std::vector<ColumnValue>& values);
    size_t getRowsCount() const;
    void info() const;
    void printHeader() const;
    void printByLine(size_t first, size_t last) const;
    void print() const;
    void printHead() const;
    void printTail() const;

    // CSV Methods
    static std::unique_ptr<CDataframe> loadFromCSV(
        const std::string& filename,
        const std::vector<ColumnType>& types
    );
    static std::unique_ptr<CDataframe> loadFromCSVAuto(const std::string& filename);
    void saveToCSV(const std::string& filename) const;

    // Legacy Methods (keeping for compatibility)
    void display();
    void head(std::optional<int> row = std::nullopt);
    void tail(std::optional<int> row = std::nullopt);
    void displayCol(const std::vector<std::shared_ptr<Column>>& col);

    bool addCol(Column *col);
    bool rmCol(Column *col);
    bool addRow(const std::vector<int>& row);
    bool rmRow(const int idx);
    bool renameCol(Column* col, const std::string& newName);
    bool exist(const int val);
    bool replaceValue(const Column& col, const int index, const int newVal);

    // Helper
    int sizeBiggestCol();
    std::shared_ptr<Column> getColumnByName(const std::string& name);
    std::shared_ptr<Column> getColumnByIndex(size_t index);
};


