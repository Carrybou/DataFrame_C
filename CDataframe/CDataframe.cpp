#include <iostream>
#include <optional>
#include <algorithm>
#include <sstream>

#include "CDataframe.h"

// ===== CONSTRUCTORS =====

CDataframe::CDataframe() 
{
    this->columns = std::vector<std::shared_ptr<Column>>();
}

CDataframe::CDataframe(const std::vector<ColumnType>& types)
{
    for (size_t i = 0; i < types.size(); ++i) {
        auto col = std::make_shared<Column>("col_" + std::to_string(i), types[i]);
        this->columns.push_back(col);
    }
}

CDataframe::CDataframe(const std::vector<Column> &cols)
{
    for (const auto& col : cols) {
        auto shared_col = std::make_shared<Column>(col);
        this->columns.push_back(shared_col);
    }
}

CDataframe::CDataframe(std::initializer_list<Column> cols)
{
    int maxSize = 0;
    for (const auto& c : cols) {
        maxSize = std::max(maxSize, c.getSize());
    }
    
    for (const auto& c : cols) {
        auto shared_col = std::make_shared<Column>(c);
        this->columns.push_back(shared_col);
    }
}

CDataframe::~CDataframe()
{
}

// ===== DISPLAY =====

void CDataframe::print(std::optional<int> firstRowOpt, std::optional<int> lastRowOpt, const std::vector<Column>* colOpt)
{
    const int total = this->sizeBiggestCol();
    const int firstRow = firstRowOpt.has_value() ? std::max(0, total - firstRowOpt.value()) : 0;
    const int lastRow  = lastRowOpt.has_value()  ? std::min(total, lastRowOpt.value())      : total;

    const auto& cols = colOpt ? *colOpt : [this]() -> std::vector<Column> {
        std::vector<Column> v;
        for (const auto& c : this->columns) {
            v.push_back(*c);
        }
        return v;
    }();

    // r=0 => header, r>=1 => data row r-1
    for (int r = 0; r < (lastRow - firstRow) + 2; ++r) {
        if (r == 0) {
            std::cout << "[H] ";
            for (const Column& c : cols) {
                std::cout << c.getName() << " ";
            }
            std::cout << "\n\n";
            continue;
        }

        const int row = firstRow + (r - 1);
        if (row >= total) break;

        std::cout << "[" << row << "] ";
        for (const Column& c : cols) {
            auto v = c.getValueAt(row); // optional<ColumnValue>
            if (v.has_value())
                std::cout << c.valueToString(static_cast<size_t>(row));
            else
                std::cout << "NULL";
            std::cout << " ";
        }
        std::cout << "\n\n";
    }
}

void CDataframe::display()
{
    print(std::nullopt, std::nullopt, nullptr);
}

void CDataframe::tail(std::optional<int> rowOpt)
{
    print(rowOpt.value_or(5), std::nullopt, nullptr);
}

void CDataframe::head(std::optional<int> rowOpt)
{
    print(std::nullopt, rowOpt.value_or(5), nullptr);
}

void CDataframe::displayCol(const std::vector<Column>& col)
{
    print(std::nullopt, std::nullopt, &col);
}

void CDataframe::printHeader() const
{
    for (size_t i = 0; i < this->columns.size(); ++i) {
        std::cout << this->columns[i]->getName();
        if (i < this->columns.size() - 1) std::cout << ",";
    }
    std::cout << std::endl;
}

// ===== OPERATION =====

bool CDataframe::deleteColumn(const std::string& colName)
{
    auto it = std::remove_if(this->columns.begin(), this->columns.end(),
                             [&colName](const std::shared_ptr<Column>& col) {
                                 return col->getName() == colName;
                             });
    if (it != this->columns.end()) {
        this->columns.erase(it, this->columns.end());
        return true;
    }
    return false;
}

void CDataframe::setColumnNames(const std::vector<std::string>& names)
{
    for (size_t i = 0; i < names.size() && i < this->columns.size(); ++i) {
        this->columns[i]->setName(names[i]);
    }
}

bool CDataframe::insertColumns(const std::vector<Column*>& cols)
{
    for (const auto& col : cols) {
        if (col == nullptr) {
            return false;
        }
        auto shared_col = std::make_shared<Column>(*col);
        this->columns.push_back(shared_col);
    }
    return true;
}

bool CDataframe::insertColumn(Column* col)
{
    if (col == nullptr) {
        return false;
    }
    auto shared_col = std::make_shared<Column>(*col);
    this->columns.push_back(shared_col);
    return true;
}

bool CDataframe::insertRows(const std::vector<std::vector<ColumnValue>>& rows)
{
    for (const auto& row : rows) {
        if (row.size() != this->columns.size()) {
            return false;
        }
        for (size_t i = 0; i < row.size(); ++i) {
            this->columns[i]->insertValue(row[i]);
        }
    }
    return true;
}

bool CDataframe::insertRow(const std::vector<ColumnValue>& values)
{
    if (values.size() != this->columns.size()) {
        return false;
    }

    for (size_t i = 0; i < values.size(); ++i) {
        this->columns[i]->insertValue(values[i]);
    }

    return true;
}

bool CDataframe::deleteRow(const int idx)
{
    if (idx < 0 || idx >= this->sizeBiggestCol())
        return false;

    for (size_t i = 0; i < this->columns.size(); ++i) {
        this->columns[i]->removeValue(idx);
    }

    return true;
}

bool CDataframe::renameCol(Column* col, const std::string& newName)
{
    for (size_t i = 0; i < this->columns.size(); ++i) {
        if (this->columns[i]->getName() == col->getName()) {
            return this->columns[i]->setName(newName);
        }
    }
    return false;
}

bool CDataframe::exist(const int val)
{
    for (size_t i = 0; i < this->columns.size(); ++i) {
        if (this->columns[i]->occurence(static_cast<int32_t>(val)) > 0) {
            return true;
        }
    }

    return false;
}

bool CDataframe::replaceValue(const Column& col, const int index, const int newVal)
{
    for (size_t i = 0; i < this->columns.size(); ++i) {
        if (this->columns[i]->getName() == col.getName()) {
            return this->columns[i]->accessReplaceValue(index, static_cast<int32_t>(newVal));
        }
    }
    return false;
}

// ===== STATISTICS & INFO =====

size_t CDataframe::getColumnsCount() const
{
    return this->columns.size();
}

size_t CDataframe::getRowsCount() const
{
    if (this->columns.empty()) {
        return 0;
    }
    return this->columns[0]->getSize();
}

void CDataframe::info() const
{
    std::cout << "DataFrame Information:" << std::endl;
    std::cout << "Rows: " << this->getRowsCount() << std::endl;
    std::cout << "Columns: " << this->getColumnsCount() << std::endl;
    std::cout << "\nColumn Details:" << std::endl;
    for (size_t i = 0; i < this->columns.size(); ++i) {
        std::cout << "[" << i << "] " << this->columns[i]->getName() 
                  << " (size: " << this->columns[i]->getSize() << ")" << std::endl;
    }
}

int CDataframe::numberOfRows()
{
    return static_cast<int>(this->getRowsCount());
}

int CDataframe::numberOfCols()
{
    return static_cast<int>(this->getColumnsCount());
}

int CDataframe::numberOfCellsEqualTo(int x)
{
    int count = 0;
    for (const auto& col : this->columns) {
        count += col->occurence(static_cast<int32_t>(x));
    }
    return count;
}

int CDataframe::numberOfCellsGreaterThan(int x)
{
    int count = 0;
    for (const auto& col : this->columns) {
        count += col->numberGreaterThan(static_cast<int32_t>(x));
    }
    return count;
}

int CDataframe::numberOfCellsLowerThan(int x)
{
    int count = 0;
    for (const auto& col : this->columns) {
        count += col->numberLowerThan(static_cast<int32_t>(x));
    }
    return count;
}

// ===== CSV METHODS =====

std::unique_ptr<CDataframe> CDataframe::loadFromCSV(
    const std::string& filename,
    const std::vector<ColumnType>& types)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    auto df = std::make_unique<CDataframe>(types);
    std::string line;

    if (std::getline(file, line)) {
        std::vector<std::string> headers;
        std::stringstream ss(line);
        std::string header;
        while (std::getline(ss, header, ',')) {
            headers.push_back(header);
        }
        df->setColumnNames(headers);
    }

    // Read data rows
    while (std::getline(file, line)) {
        std::vector<ColumnValue> values;
        std::stringstream ss(line);
        std::string value;
        size_t col_idx = 0;

        while (std::getline(ss, value, ',') && col_idx < types.size()) {
            try {
                switch (types[col_idx]) {
                    case ColumnType::INT:
                        values.push_back(static_cast<int32_t>(std::stoi(value)));
                        break;
                    case ColumnType::DOUBLE:
                        values.push_back(std::stod(value));
                        break;
                    case ColumnType::STRING:
                        values.push_back(value);
                        break;
                    case ColumnType::CHAR:
                        values.push_back(value.empty() ? ' ' : value[0]);
                        break;
                    default:
                        values.push_back(std::monostate{});
                        break;
                }
            } catch (...) {
                values.push_back(std::monostate{});
            }
            col_idx++;
        }

        if (!values.empty()) {
            df->insertRow(values);
        }
    }

    file.close();
    return df;
}

std::unique_ptr<CDataframe> CDataframe::loadFromCSVAuto(const std::string& filename)
{
    std::vector<ColumnType> types = {ColumnType::INT};
    return loadFromCSV(filename, types);
}

void CDataframe::saveToCSV(const std::string& filename) const
{
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot create file: " + filename);
    }

    for (size_t i = 0; i < this->columns.size(); ++i) {
        file << this->columns[i]->getName();
        if (i < this->columns.size() - 1) file << ",";
    }
    file << "\n";

    for (size_t row = 0; row < this->getRowsCount(); ++row) {
        for (size_t col = 0; col < this->columns.size(); ++col) {
            auto val = this->columns[col]->getValueAt(row);
            if (val) {
                file << this->columns[col]->valueToString(row);
            } else {
                file << "NULL";
            }
            if (col < this->columns.size() - 1) file << ",";
        }
        file << "\n";
    }

    file.close();
}

// ===== HELPER =====

int CDataframe::sizeBiggestCol()
{
    int max = 0;
    for (const auto& col : this->columns) {
        if (col->getSize() > max) max = col->getSize();
    }
    return max;
}

std::shared_ptr<Column> CDataframe::getColumnByName(const std::string& name)
{
    for (const auto& col : this->columns) {
        if (col->getName() == name) {
            return col;
        }
    }
    return nullptr;
}

std::shared_ptr<Column> CDataframe::getColumnByIndex(size_t index)
{
    if (index < this->columns.size()) {
        return this->columns[index];
    }
    return nullptr;
}
