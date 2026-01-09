#include <iostream>

#include "Column/Column.h"
#include "CDataframe/CDataframe.h"

int main(int argc, char const *argv[])
{
     // Test de la partie 4
    std::cout << "\n=== TEST PARTIE 4 - Entier ===" << std::endl;
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

    // Test de la partie 6.1 et 6.2 - sort() et printSorted()
    std::cout << "\n=== TEST PARTIE 6.1 & 6.2 - SORT & PRINTSORTED ===" << std::endl;
    
    Column col("TestSort");
    col.insertValue(5);
    col.insertValue(2);
    col.insertValue(8);
    col.insertValue(1);
    col.insertValue(9);
    
    std::cout << "Avant tri (display): ";
    col.display();
    std::cout << std::endl;
    
    std::cout << "\nAffichage trié croissant (printSorted):" << std::endl;
    col.printSorted(true);
    
    std::cout << "\nAffichage trié décroissant (printSorted):" << std::endl;
    col.printSorted(false);
    
    std::cout << "\nAprès tri, display() affiche toujours l'ordre original:" << std::endl;
    col.display();
    std::cout << std::endl;
    
    std::cout << "\n=== FIN TEST 6.1 & 6.2 ===" << std::endl;

    return 0;
}
