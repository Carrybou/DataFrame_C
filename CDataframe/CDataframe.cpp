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

void CDataframe::display(std::optional<int> rowOpt, const std::vector<Column>* colOpt)
{
    const int rows = rowOpt.value_or(this->sizeBiggestCol());
    const auto& cols = colOpt ? *colOpt : this->columns;

    for (int r = 0; r < rows; ++r) {
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

void CDataframe::head(std::optional<int> rowOpt)
{
    display(rowOpt.value_or(5));
}

void CDataframe::byCol(const std::vector<Column>& col)
{
    display(std::nullopt, &col);
}