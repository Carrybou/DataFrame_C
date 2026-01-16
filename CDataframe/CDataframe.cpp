// ========================= CDataframe.cpp =========================
#include <iostream>
#include <optional>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <cctype>
#include <limits>

#include "CDataframe.h"

// ----------------- CSV helpers (minimum) -----------------

static std::vector<std::string> splitCsvLine(const std::string& line)
{
    std::vector<std::string> out;
    std::stringstream ss(line);
    std::string token;
    while (std::getline(ss, token, ',')) out.push_back(token);
    return out;
}

static std::string trim(const std::string& s)
{
    size_t a = 0, b = s.size();
    while (a < b && std::isspace(static_cast<unsigned char>(s[a]))) ++a;
    while (b > a && std::isspace(static_cast<unsigned char>(s[b-1]))) --b;
    return s.substr(a, b - a);
}

static bool isNullToken(const std::string& raw)
{
    const std::string s = trim(raw);
    return s.empty() || s == "NULL" || s == "null" || s == "NaN";
}

static bool parseInt64(const std::string& raw, long long& out)
{
    std::string s = trim(raw);
    if (s.empty()) return false;

    size_t i = 0;
    if (s[0] == '+' || s[0] == '-') i = 1;
    if (i >= s.size()) return false;

    for (; i < s.size(); ++i)
        if (!std::isdigit(static_cast<unsigned char>(s[i])))
            return false;

    try {
        size_t idx = 0;
        out = std::stoll(s, &idx, 10);
        return idx == s.size();
    } catch (...) {
        return false;
    }
}

static bool parseUInt64(const std::string& raw, unsigned long long& out)
{
    std::string s = trim(raw);
    if (s.empty()) return false;

    for (char c : s)
        if (!std::isdigit(static_cast<unsigned char>(c)))
            return false;

    try {
        size_t idx = 0;
        out = std::stoull(s, &idx, 10);
        return idx == s.size();
    } catch (...) {
        return false;
    }
}

static bool parseDouble(const std::string& raw, double& out)
{
    std::string s = trim(raw);
    if (s.empty()) return false;

    try {
        size_t idx = 0;
        out = std::stod(s, &idx);
        return idx == s.size();
    } catch (...) {
        return false;
    }
}

static ColumnValue parseByType(const std::string& raw, ColumnType t)
{
    std::string s = trim(raw);
    if (isNullToken(s)) return std::monostate{};

    try {
        switch (t) {
            case ColumnType::NULLVAL:
                return std::monostate{};

            case ColumnType::STRING:
                return s;

            case ColumnType::DOUBLE:
                return static_cast<double>(std::stod(s));

            case ColumnType::FLOAT:
                return static_cast<float>(std::stof(s));

            case ColumnType::INT:
                return static_cast<int32_t>(std::stol(s));

            case ColumnType::UINT:
                return static_cast<uint32_t>(std::stoul(s));

            case ColumnType::SHORT:
                return static_cast<int16_t>(std::stoi(s));

            case ColumnType::USHORT:
                return static_cast<uint16_t>(std::stoul(s));

            case ColumnType::LONG:
                return static_cast<int64_t>(std::stoll(s));

            case ColumnType::ULONG:
                return static_cast<uint64_t>(std::stoull(s));

            case ColumnType::CHAR: {
                long long v;
                if (parseInt64(s, v)) return static_cast<int8_t>(v);
                return s.empty() ? static_cast<int8_t>(0) : static_cast<int8_t>(s[0]);
            }

            case ColumnType::UCHAR: {
                unsigned long long v;
                if (parseUInt64(s, v)) return static_cast<uint8_t>(v);
                return s.empty() ? static_cast<uint8_t>(0) : static_cast<uint8_t>(s[0]);
            }

            case ColumnType::OBJECT:
                // CSV -> on stocke texte brut dans std::any
                return std::any(s);

            default:
                return std::monostate{};
        }
    } catch (...) {
        return std::monostate{};
    }
}

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
    for (const auto& c : cols) {
        this->columns.push_back(std::make_shared<Column>(c));
    }
}

CDataframe::~CDataframe() {}

// ===== DISPLAY =====

void CDataframe::print(std::optional<int> firstRowOpt, std::optional<int> lastRowOpt, const std::vector<Column>* colOpt)
{
    const int total = this->sizeBiggestCol();
    const int firstRow = firstRowOpt.has_value() ? std::max(0, total - firstRowOpt.value()) : 0;
    const int lastRow  = lastRowOpt.has_value()  ? std::min(total, lastRowOpt.value())      : total;

    const auto& cols = colOpt ? *colOpt : [this]() -> std::vector<Column> {
        std::vector<Column> v;
        v.reserve(this->columns.size());
        for (const auto& c : this->columns) v.push_back(*c);
        return v;
    }();

    // header
    std::cout << "[H] ";
    for (const Column& c : cols) std::cout << c.getName() << " ";
    std::cout << "\n\n";

    for (int row = firstRow; row < lastRow; ++row) {
        if (row >= total) break;

        std::cout << "[" << row << "] ";
        for (const Column& c : cols) {
            auto v = c.getValueAt(row);
            if (v.has_value()) std::cout << c.valueToString(static_cast<size_t>(row));
            else std::cout << "NULL";
            std::cout << " ";
        }
        std::cout << "\n\n";
    }
}

void CDataframe::display() { print(std::nullopt, std::nullopt, nullptr); }
void CDataframe::tail(std::optional<int> rowOpt) { print(rowOpt.value_or(5), std::nullopt, nullptr); }
void CDataframe::head(std::optional<int> rowOpt) { print(std::nullopt, rowOpt.value_or(5), nullptr); }
void CDataframe::displayCol(const std::vector<Column>& col) { print(std::nullopt, std::nullopt, &col); }

void CDataframe::printHeader() const
{
    for (size_t i = 0; i < this->columns.size(); ++i) {
        std::cout << this->columns[i]->getName();
        if (i + 1 < this->columns.size()) std::cout << ",";
    }
    std::cout << "\n";
}

// ===== OPERATION =====

bool CDataframe::deleteColumn(const std::string& colName)
{
    auto it = std::remove_if(this->columns.begin(), this->columns.end(),
        [&colName](const std::shared_ptr<Column>& col) {
            return col && col->getName() == colName;
        });

    if (it == this->columns.end()) return false;

    this->columns.erase(it, this->columns.end());
    return true;
}

void CDataframe::setColumnNames(const std::vector<std::string>& names)
{
    for (size_t i = 0; i < names.size() && i < this->columns.size(); ++i)
        this->columns[i]->setName(names[i]);
}

bool CDataframe::insertColumns(const std::vector<Column*>& cols)
{
    for (const auto& col : cols) {
        if (!col) return false;
        this->columns.push_back(std::make_shared<Column>(*col));
    }
    return true;
}

bool CDataframe::insertColumn(Column* col)
{
    if (!col) return false;
    this->columns.push_back(std::make_shared<Column>(*col));
    return true;
}

bool CDataframe::insertRows(const std::vector<std::vector<ColumnValue>>& rows)
{
    for (const auto& row : rows) {
        if (row.size() != this->columns.size()) return false;
        if (!this->insertRow(row)) return false;
    }
    return true;
}

bool CDataframe::insertRow(const std::vector<ColumnValue>& values)
{
    if (values.size() != this->columns.size())
        return false;

    // tentative d'insertion + rollback si une colonne refuse
    for (size_t i = 0; i < values.size(); ++i) {
        if (!this->columns[i]->insertValueAuto(values[i])) {
            for (size_t j = 0; j < i; ++j)
                this->columns[j]->removeValue(this->columns[j]->getSize() - 1);
            return false;
        }
    }
    return true;
}

bool CDataframe::deleteRow(const int idx)
{
    if (idx < 0 || idx >= this->sizeBiggestCol()) return false;

    for (auto& c : this->columns) c->removeValue(idx);
    return true;
}

bool CDataframe::renameCol(Column* col, const std::string& newName)
{
    if (!col || newName.empty()) return false;

    for (auto& c : this->columns) {
        if (c->getName() == col->getName())
            return c->setName(newName);
    }
    return false;
}

bool CDataframe::exist(const int val)
{
    for (auto& c : this->columns)
        if (c->occurence(static_cast<int32_t>(val)) > 0)
            return true;
    return false;
}

bool CDataframe::replaceValue(const Column& col, const int index, const int newVal)
{
    for (auto& c : this->columns) {
        if (c->getName() == col.getName())
            return c->accessReplaceValue(index, static_cast<int32_t>(newVal));
    }
    return false;
}

// ===== STATISTICS & INFO =====

size_t CDataframe::getColumnsCount() const { return this->columns.size(); }

size_t CDataframe::getRowsCount() const
{
    if (this->columns.empty()) return 0;
    return static_cast<size_t>(this->columns[0]->getSize());
}

int CDataframe::numberOfRows() { return static_cast<int>(this->getRowsCount()); }
int CDataframe::numberOfCols() { return static_cast<int>(this->getColumnsCount()); }

int CDataframe::numberOfCellsEqualTo(int x)
{
    int count = 0;
    for (const auto& col : this->columns)
        count += col->occurence(static_cast<int32_t>(x));
    return count;
}

int CDataframe::numberOfCellsGreaterThan(int x)
{
    int count = 0;
    for (const auto& col : this->columns)
        count += col->numberGreaterThan(static_cast<int32_t>(x));
    return count;
}

int CDataframe::numberOfCellsLowerThan(int x)
{
    int count = 0;
    for (const auto& col : this->columns)
        count += col->numberLowerThan(static_cast<int32_t>(x));
    return count;
}

void CDataframe::info() const
{
    std::cout << "DataFrame Information:\n";
    std::cout << "Rows: " << this->getRowsCount() << "\n";
    std::cout << "Columns: " << this->getColumnsCount() << "\n\n";
    std::cout << "Column Details:\n";
    for (size_t i = 0; i < this->columns.size(); ++i) {
        std::cout << "[" << i << "] " << this->columns[i]->getName()
                  << " (size: " << this->columns[i]->getSize() << ")\n";
    }
}

// ===== CSV METHODS =====

std::unique_ptr<CDataframe> CDataframe::loadFromCSV(
    const std::string& filename,
    const std::vector<ColumnType>& types)
{
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Cannot open file: " + filename);

    auto df = std::make_unique<CDataframe>(types);

    std::string line;
    if (std::getline(file, line)) {
        auto headers = splitCsvLine(line);
        df->setColumnNames(headers);
    }

    while (std::getline(file, line)) {
        auto cells = splitCsvLine(line);

        std::vector<ColumnValue> values;
        values.reserve(types.size());

        for (size_t i = 0; i < types.size(); ++i) {
            std::string cell = (i < cells.size()) ? cells[i] : "";
            values.push_back(parseByType(cell, types[i]));
        }

        df->insertRow(values);
    }

    return df;
}

std::unique_ptr<CDataframe> CDataframe::loadFromCSVAuto(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Cannot open file: " + filename);

    std::string headerLine;
    if (!std::getline(file, headerLine))
        throw std::runtime_error("Empty CSV: " + filename);

    auto headers = splitCsvLine(headerLine);
    const size_t ncols = headers.size();
    if (ncols == 0)
        throw std::runtime_error("No columns in header: " + filename);

    std::vector<bool> seenString(ncols, false);
    std::vector<bool> seenFloat(ncols, false);
    std::vector<bool> seenNegative(ncols, false);

    std::vector<long long> minS(ncols, std::numeric_limits<long long>::max());
    std::vector<long long> maxS(ncols, std::numeric_limits<long long>::min());
    std::vector<unsigned long long> maxU(ncols, 0);

    std::string line;
    int sample = 0;

    while (sample < 200 && std::getline(file, line)) {
        auto cells = splitCsvLine(line);

        for (size_t c = 0; c < ncols; ++c) {
            std::string s = (c < cells.size()) ? trim(cells[c]) : "";
            if (isNullToken(s)) continue;

            double d = 0.0;
            if (!parseDouble(s, d)) {
                seenString[c] = true;
                continue;
            }

            long long si = 0;
            unsigned long long ui = 0;

            if (parseInt64(s, si)) {
                if (si < 0) {
                    seenNegative[c] = true;
                    minS[c] = std::min(minS[c], si);
                    maxS[c] = std::max(maxS[c], si);
                } else {
                    maxU[c] = std::max(maxU[c], static_cast<unsigned long long>(si));
                    maxS[c] = std::max(maxS[c], si);
                }
            } else if (parseUInt64(s, ui)) {
                maxU[c] = std::max(maxU[c], ui);
            } else {
                seenFloat[c] = true;
            }
        }

        ++sample;
    }

    std::vector<ColumnType> types(ncols, ColumnType::STRING);

    for (size_t c = 0; c < ncols; ++c) {
        if (seenString[c]) { types[c] = ColumnType::STRING; continue; }
        if (seenFloat[c])  { types[c] = ColumnType::DOUBLE; continue; }

        if (seenNegative[c]) {
            if (minS[c] >= std::numeric_limits<int8_t>::min()  && maxS[c] <= std::numeric_limits<int8_t>::max())
                types[c] = ColumnType::CHAR;
            else if (minS[c] >= std::numeric_limits<int16_t>::min() && maxS[c] <= std::numeric_limits<int16_t>::max())
                types[c] = ColumnType::SHORT;
            else if (minS[c] >= std::numeric_limits<int32_t>::min() && maxS[c] <= std::numeric_limits<int32_t>::max())
                types[c] = ColumnType::INT;
            else
                types[c] = ColumnType::LONG;
        } else {
            if (maxU[c] <= std::numeric_limits<uint8_t>::max())
                types[c] = ColumnType::UCHAR;
            else if (maxU[c] <= std::numeric_limits<uint16_t>::max())
                types[c] = ColumnType::USHORT;
            else if (maxU[c] <= std::numeric_limits<uint32_t>::max())
                types[c] = ColumnType::UINT;
            else
                types[c] = ColumnType::ULONG;
        }
    }

    return loadFromCSV(filename, types);
}

void CDataframe::saveToCSV(const std::string& filename) const
{
    std::ofstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Cannot create file: " + filename);

    for (size_t i = 0; i < this->columns.size(); ++i) {
        file << this->columns[i]->getName();
        if (i + 1 < this->columns.size()) file << ",";
    }
    file << "\n";

    for (size_t row = 0; row < this->getRowsCount(); ++row) {
        for (size_t col = 0; col < this->columns.size(); ++col) {
            auto val = this->columns[col]->getValueAt(static_cast<int>(row));
            if (val.has_value()) file << this->columns[col]->valueToString(row);
            else file << "NULL";

            if (col + 1 < this->columns.size()) file << ",";
        }
        file << "\n";
    }
}

// ===== HELPER =====

int CDataframe::sizeBiggestCol()
{
    int max = 0;
    for (const auto& col : this->columns)
        if (col->getSize() > max) max = col->getSize();
    return max;
}

std::shared_ptr<Column> CDataframe::getColumnByName(const std::string& name)
{
    for (const auto& col : this->columns)
        if (col->getName() == name) return col;
    return nullptr;
}

std::shared_ptr<Column> CDataframe::getColumnByIndex(size_t index)
{
    if (index < this->columns.size()) return this->columns[index];
    return nullptr;
}
