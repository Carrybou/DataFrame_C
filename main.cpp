#include <iostream>

#include "Column/Column.h"
#include "CDataframe/CDataframe.h"

int main(int argc, char const *argv[])
{
    Column c1 = Column("c1");
    Column c2 = Column("c2");
    Column c3 = Column("c3");
    Column c4 = Column("c4");

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

    c3.insertValue(19);

    c4.insertValue(324);
    c4.insertValue(1);
    c4.insertValue(6);
    c4.insertValue(6);
    c4.insertValue(6);

    CDataframe cd = CDataframe({c1, c2, c3, c4});

    cd.rmCol(&c2);

    cd.addCol(&c2);

    cd.renameCol(&c2, "test");

    cd.display();
    std::cout << "--------\n";

    int val = 6;
    std::cout << "does " << val << " exist ? " << (cd.exist(val) ? "true" : "false") << std::endl;

    return 0;
}
