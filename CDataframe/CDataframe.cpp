#include <iostream>
#include <optional>
#include <algorithm> // std::max

#include "CDataframe.h"

CDataframe::CDataframe()
{
    this->columns = std::vector<Column>();
}

CDataframe::CDataframe(std::initializer_list<Column> cols)
    : columns(cols)
{
    int maxSize = 0;
    for (const auto& c : columns) maxSize = std::max(maxSize, c.getSize());
    for (auto& c : columns)
        while (c.getSize() < maxSize) c.insertValue(std::nullopt); // insert NULL
}

CDataframe::~CDataframe()
{
}

/*
 Display
--------------------------------------------
*/

void CDataframe::print(std::optional<int> firstRowOpt, std::optional<int> lastRowOpt, const std::vector<Column>* colOpt)
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
    }
}

void CDataframe::display()
{
    print(std::nullopt, std::nullopt);
}

void CDataframe::tail(std::optional<int> rowOpt)
{
    print(rowOpt.value_or(5), std::nullopt);
}

void CDataframe::head(std::optional<int> rowOpt)
{
    // head = de 0 à n-1
    print(std::nullopt, rowOpt.value_or(5));
}

void CDataframe::displayCol(const std::vector<Column>& col)
{
    print(std::nullopt, std::nullopt, &col);
}

/*
 Operation
--------------------------------------------
*/

bool CDataframe::addCol(Column* col)
{
    if (col == nullptr) return false;

    this->columns.push_back(*col);
    return true;
}

bool CDataframe::rmCol(Column* optCol)
{
    if (optCol == nullptr) return false;

    for (auto it = this->columns.begin(); it != this->columns.end(); ) {
        if (it->getName() == optCol->getName())
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
            this->columns.at(i).insertValue(std::nullopt);
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

    for (size_t i = 0; i < this->columns.size(); i++)
    {
        if (this->columns.at(i).getName() == col->getName())
            return this->columns.at(i).setName(newName);
    }

    return false;
}

bool CDataframe::exist(const int val)
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
    for (const Column& x : this->columns)
        if (x.getSize() > max) max = x.getSize();
    return max;
}
