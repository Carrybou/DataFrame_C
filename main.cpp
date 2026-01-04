#include <iostream>

#include "Column/Column.h"
#include "CDataframe/CDataframe.h"

int main(int argc, char const *argv[])
{
    Column c1 = Column("c1");
    Column c2 = Column("c2");

    c1.insertValue(56);
    c1.insertValue(6);
    c1.insertValue(5);
    c1.insertValue(5);
    c1.insertValue(5);
    c1.insertValue(5);
    c1.insertValue(5);
    c1.insertValue(5);

    c2.insertValue(235);
    c2.insertValue(43);
    c2.insertValue(4);

    CDataframe cd = CDataframe({c1, c2});

    std::vector<Column> subset = {c1};

    cd.byCol(subset);

    return 0;
}
