#include <iostream>
#include <optional>

#include "CDataframe.h"

CDataframe::CDataframe() 
{
    this->columns = std::vector<Column>();
}

CDataframe::CDataframe(std::initializer_list<Column> cols)
{
    this->columns = std::vector<Column>();

    for (Column col : cols)
    {
        this->columns.push_back(col);
    }
}

CDataframe::~CDataframe()
{
}

void CDataframe::addCol(Column* col)
{
    this->columns.push_back(*col);
}

void CDataframe::print(std::optional<int> firstRowOpt, std::optional<int> lastRowOpt, const std::vector<Column>* colOpt)
{
    const int firstRow = firstRowOpt.has_value() ? this->sizeBiggestCol() - firstRowOpt.value() : 0;
    const int lastRow = lastRowOpt.has_value() ? lastRowOpt.value() : this->sizeBiggestCol();
    const auto& cols = colOpt ? *colOpt : this->columns;

    for (int r = firstRow; r < lastRow; ++r) {
        std::cout << "[" << r << "] ";
        for (const Column& c : cols) {
            auto v = c.getValueAt(r); // optional<int>
            if (v) std::cout << *v;
            else   std::cout << "NULL";
            std::cout << " ";
        }
        std::cout << "\n\n";
    }
}

int CDataframe::sizeBiggestCol()
{
    int max = 0;
    for (const Column& x : this->columns)
        if (x.getSize() > max) max = x.getSize();
    return max;
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

void CDataframe::byCol(const std::vector<Column>& col)
{
    print(std::nullopt, std::nullopt, &col);
}