#include <iostream>
#include <optional>

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
        while (c.getSize() < maxSize) c.insertValue(std::nullopt);
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
    const int firstRow = firstRowOpt.has_value() ? this->sizeBiggestCol() - firstRowOpt.value() : 0;
    const int lastRow = lastRowOpt.has_value() ? lastRowOpt.value() : this->sizeBiggestCol();
    const auto& cols = colOpt ? *colOpt : this->columns;

    for (int r = firstRow; r < lastRow + 1; ++r) {
        std::cout << "[" << r << "] ";
        for (const Column& c : cols) {
            if (r == 0)
            {
                std::cout << c.getName() << " ";
            }
            else
            {
                int optR = r - 1;
                auto v = c.getValueAt(optR); // optional<int>
                if (v) std::cout << *v;
                else   std::cout << "NULL";
                std::cout << " ";
            }
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

bool CDataframe::rmCol(Column *optCol)
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
    if (row.size() > this->columns.size() || row.empty())
        return false;

    for (size_t i = 0; i < this->columns.size(); i++)
    {
        if (i >= row.size())
            this->columns.at(i).insertValue(std::nullopt);
        else
            this->columns.at(i).insertValue(row[i]);

        return true;
    }

    return false;
}

bool CDataframe::rmRow(const int idx)
{
    if (idx < 0 || idx >= this->sizeBiggestCol())
        return false;

    for (size_t i = 0; i < this->columns.size(); i++)
    {
        this->columns.at(i).removeValue(idx);
        return true;
    }

    return false;
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
    for (size_t i = 0; i < this->columns.size(); i++)
    {
        if (this->columns.at(i).exist(val))
        {
            return true;
        }
    }

    return false;
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
