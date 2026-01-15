#include <iostream>
#include <optional>
#include <algorithm>
#include <sstream>

#include "CDataframe.h"

CDataframe::CDataframe()
{
    this->columns = std::vector<std::shared_ptr<Column>>();
}

CDataframe::CDataframe(const std::vector<ColumnType>& types)
{
    // Create columns with specified types
    for (size_t i = 0; i < types.size(); ++i) {
        auto col = std::make_shared<Column>("col_" + std::to_string(i));
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
    for (const auto& c : columns) maxSize = std::max(maxSize, c.getSize());
    for (auto& c : columns)
        while (c.getSize() < maxSize) c.insertValue(std::nullopt); // insert NULL
}

CDataframe::~CDataframe()
{
}



void CDataframe::deleteColumn(const std::string& colName)
{
    for (auto it = this->columns.begin(); it != this->columns.end(); ++it) {
        if ((*it)->getName() == colName) {
            this->columns.erase(it);
            return;
        }
    }
}

void CDataframe::setColumnNames(const std::vector<std::string>& names)
{
    for (size_t i = 0; i < names.size() && i < this->columns.size(); ++i) {
        this->columns[i]->setName(names[i]);
    }
}

size_t CDataframe::getColumnsCount() const
{
    return this->columns.size();
}

bool CDataframe::insertRow(const std::vector<ColumnValue>& values)
{
    // MIN FIX: logique de firstRow (ton calcul sizeBiggestCol() - firstRowOpt était bizarre)
    // Ici: firstRowOpt = nombre de lignes à afficher depuis la fin (tail)
    const int total = this->sizeBiggestCol();
    const int firstRow = firstRowOpt.has_value() ? std::max(0, total - firstRowOpt.value()) : 0;
    const int lastRow  = lastRowOpt.has_value()  ? std::min(total, lastRowOpt.value())      : total;

    const auto& cols = colOpt ? *colOpt : this->columns;

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
    if (values.size() != this->columns.size()) {
        return false;
    }

    for (size_t i = 0; i < values.size(); ++i) {
        
        if (std::holds_alternative<int>(values[i])) {
            this->columns[i]->insertValue(std::get<int>(values[i]));
        }
    }

    return true;
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

void CDataframe::printHeader() const
{
    for (size_t i = 0; i < this->columns.size(); ++i) {
        std::cout << this->columns[i]->getName();
        if (i < this->columns.size() - 1) std::cout << ",";
    }
    std::cout << std::endl;
}

void CDataframe::printByLine(size_t first, size_t last) const
{
    size_t rows = this->getRowsCount();
    
    if (first > last || first >= rows) {
        return;
    }
    
    if (last > rows) {
        last = rows;
    }

    // Print header
    this->printHeader();

    // Print rows
    for (size_t row = first; row < last; ++row) {
        for (size_t col = 0; col < this->columns.size(); ++col) {
            auto val = this->columns[col]->getValueAt(row);
            if (val) {
                std::cout << *val;
            } else {
                std::cout << "NULL";
            }
            if (col < this->columns.size() - 1) std::cout << ",";
        }
        std::cout << std::endl;
    }
}

void CDataframe::print() const
{
    this->printByLine(0, this->getRowsCount());
}

void CDataframe::printHead() const
{
    size_t rows = std::min((size_t)10, this->getRowsCount());
    this->printByLine(0, rows);
}

void CDataframe::printTail() const
{
    size_t rows = this->getRowsCount();
    if (rows <= 10) {
        this->printByLine(0, rows);
    } else {
        this->printByLine(rows - 10, rows);
    }
}


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

    // Read header
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
                        values.push_back(std::stoi(value));
                        break;
                    case ColumnType::DOUBLE:
                        values.push_back(std::stod(value));
                        break;
                    case ColumnType::STRING:
                        values.push_back(value);
                        break;
                    case ColumnType::CHAR:
                        values.push_back(value[0]);
                        break;
                }
            } catch (...) {
                values.push_back(0); 
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

    // Write header
    for (size_t i = 0; i < this->columns.size(); ++i) {
        file << this->columns[i]->getName();
        if (i < this->columns.size() - 1) file << ",";
    }
    file << "\n";

    // Write data
    for (size_t row = 0; row < this->getRowsCount(); ++row) {
        for (size_t col = 0; col < this->columns.size(); ++col) {
            auto val = this->columns[col]->getValueAt(row);
            if (val) {
                file << *val;
            } else {
                file << "NULL";
            }
            if (col < this->columns.size() - 1) file << ",";
        }
        file << "\n";
    }

    file.close();
}



void CDataframe::display()
{
    this->print();
}

void CDataframe::head(std::optional<int> rowOpt)
{
    int rows = rowOpt.value_or(10);
    this->printByLine(0, rows);
}

void CDataframe::tail(std::optional<int> rowOpt)
{
    int rows = rowOpt.value_or(10);
    size_t total = this->getRowsCount();
    if ((size_t)rows >= total) {
        this->printByLine(0, total);
    } else {
        this->printByLine(total - rows, total);
    }
}

void CDataframe::displayCol(const std::vector<std::shared_ptr<Column>>& col)
{
    // Print header
    for (const auto& c : col) {
        std::cout << c->getName() << " ";
    }
    std::cout << std::endl;

    // Print data
    size_t max_rows = 0;
    for (const auto& c : col) {
        max_rows = std::max(max_rows, (size_t)c->getSize());
    }

    for (size_t row = 0; row < max_rows; ++row) {
        for (const auto& c : col) {
            auto val = c->getValueAt(row);
            if (val) {
                std::cout << *val;
            } else {
                std::cout << "NULL";
            }
            std::cout << " ";
        }
        std::cout << std::endl;
    }
}

bool CDataframe::addCol(Column *col)
{
    if (col == nullptr) return false;
    auto shared_col = std::make_shared<Column>(*col);
    this->columns.push_back(shared_col);
    return true;
}

bool CDataframe::rmCol(Column* optCol)
{
    if (optCol == nullptr) return false;

    for (auto it = this->columns.begin(); it != this->columns.end(); ) {
        if ((*it)->getName() == optCol->getName())
            it = this->columns.erase(it);
        else
            ++it;
    }

    return true;
}

bool CDataframe::addRow(const std::vector<int>& row)
{
    if (row.empty() || row.size() > this->columns.size())
        return false;

    // MIN FIX: le return true était dans la boucle => ça sortait au 1er élément
    for (size_t i = 0; i < this->columns.size(); i++)
    {
        if (i >= row.size())
            this->columns[i]->insertValue(0);
        else
            this->columns.at(i).insertValue(ColumnValue(row[i])); // conversion explicite
    }

    return true;
}

bool CDataframe::rmRow(const int idx)
{
    if (idx < 0 || idx >= this->sizeBiggestCol())
        return false;

    // MIN FIX: return true était dans la boucle => ça supprimait que dans la 1ère colonne
    for (size_t i = 0; i < this->columns.size(); i++)
    {
        this->columns.at(i).removeValue(idx);
    }

    return true;
}

bool CDataframe::renameCol(Column* col, const std::string& newName)
{
    if (col == nullptr || newName.empty()) return false;

    for (size_t i = 0; i < this->columns.size(); ++i) {
        if (this->columns[i]->getName() == col->getName()) {
            this->columns[i]->setName(newName);
            return true;
        }
    }

    return false;
}

bool CDataframe::exist(const int val)
{
    for (size_t i = 0; i < this->columns.size(); ++i) {
        if (this->columns[i]->occurence(val) > 0) {
            return true;
        }
    }

    return false;
}

bool CDataframe::replaceValue(const Column& col, const int index, const int newVal)
{
    for (size_t i = 0; i < this->columns.size(); i++)
    {
        if (this->columns.at(i).exist(val))
        {
            return true;
        }
    }

    return false;
}

bool CDataframe::replaceValue(const Column& col, const int index, const int newVal)
{
    // Avec ta nouvelle Column, exist prend ColumnValue
    ColumnValue v(val);

    for (size_t i = 0; i < this->columns.size(); i++)
    {
        if (this->columns.at(i).exist(v))
            return true;
    }
    return false;
}

bool CDataframe::replaceValue(const Column& col, const int index, const int newVal)
{
    for (size_t i = 0; i < this->columns.size(); i++)
    {
        if (this->columns.at(i).getName() == col.getName())
        {
            // ta Column attend optional<ColumnValue>
            return this->columns.at(i).accessReplaceValue(index, ColumnValue(newVal));
        }
    }

    return false;
}

/*
 Statistics
--------------------------------------------
*/

int CDataframe::numberOfRows()
{
    return this->sizeBiggestCol();
}

int CDataframe::numberOfCols()
{
    return static_cast<int>(this->columns.size());
}

int CDataframe::numberOfCellsEqualTo(int x)
{
    int count = 0;
    ColumnValue v(x);
    for (const Column& col : this->columns)
        count += col.occurence(v);
    return count;
}

int CDataframe::numberOfCellsGreaterThan(int x)
{
    int count = 0;
    ColumnValue v(x);
    for (const Column& col : this->columns)
        count += col.numberGreaterThan(v);
    return count;
}

int CDataframe::numberOfCellsLowerThan(int x)
{
    int count = 0;
    ColumnValue v(x);
    for (const Column& col : this->columns)
        count += col.numberLowerThan(v);
    return count;
}

/*
 Helper
--------------------------------------------
*/

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
