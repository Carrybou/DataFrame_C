#include <iostream>

#include "Column/Column.cpp"

int main(int argc, char const *argv[])
{
    Column c1 = Column("c1");

    c1.insertValue(56);
    c1.insertValue(6);
    c1.insertValue(5);
    c1.insertValue(5);
    c1.insertValue(5);
    c1.insertValue(5);

    c1.display();

    std::cout << c1.occurence(5) << std::endl;
    std::cout << c1.numberGreaterThan(6) << std::endl;
    std::cout << c1.numberLowerThan(6) << std::endl;
    return 0;
}
