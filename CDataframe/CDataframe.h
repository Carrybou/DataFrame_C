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

    void display(std::optional<int> rowOpt = std::nullopt, const std::vector<Column>* colOpt = nullptr);
    void head(std::optional<int> row = std::nullopt);
    void byCol(const std::vector<Column>& col);
    int sizeBiggestCol();
};
