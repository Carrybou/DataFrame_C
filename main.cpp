#include <iostream>

#include "Column/Column.h"
#include "CDataframe/CDataframe.h"

int main(int argc, char const *argv[])
{
     // Test de la partie 4
    std::cout << "\n=== TEST PARTIE 4 - Entier ===" << std::endl;
    Column c1 = Column("c1", ColumnType::INT);
    Column c2 = Column("c2", ColumnType::INT);
    Column c3 = Column("c3", ColumnType::INT);
    Column c4 = Column("c4", ColumnType::INT);

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

    cd.deleteColumn("c3");

    cd.insertColumn(&c2);

    cd.renameCol(&c2, "test");

    cd.display();
    std::cout << "--------\n";

    int val = 6;
    std::cout << "does " << val << " exist ? " << (cd.exist(val) ? "true" : "false") << std::endl;

    // Test de la partie 6.1 et 6.2 - sort() et printSorted()
    std::cout << "\n=== TEST PARTIE 6.1 & 6.2 - SORT & PRINTSORTED ===" << std::endl;
    
    Column col("TestSort", ColumnType::INT);
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

    // Test de la partie 6.3 et 6.4 - eraseIndex() et checkIndex()
    std::cout << "\n=== TEST PARTIE 6.3 & 6.4 - ERASEINDEX & CHECKINDEX ===" << std::endl;
    
    Column col3("TestIndexManagement", ColumnType::INT);
    
    std::cout << "Avant d'insérer des valeurs - checkIndex(): " << col3.checkIndex() << " (attendu: -1)" << std::endl;
    
    col3.insertValue(5);
    col3.insertValue(2);
    col3.insertValue(8);
    col3.insertValue(1);
    col3.insertValue(9);
    
    std::cout << "Après insertion mais sans tri - checkIndex(): " << col3.checkIndex() << " (attendu: -1)" << std::endl;
    
    col3.sort(true);
    std::cout << "Après tri - checkIndex(): " << col3.checkIndex() << " (attendu: 1)" << std::endl;
    
    std::cout << "\nAffichage trié:" << std::endl;
    col3.printSorted(true);
    
    std::cout << "\nEffacement de l'index avec eraseIndex()..." << std::endl;
    col3.eraseIndex();
    std::cout << "Après eraseIndex() - checkIndex(): " << col3.checkIndex() << " (attendu: 0)" << std::endl;
    
    std::cout << "\nAffichage trié après eraseIndex() (va reconstruire l'index):" << std::endl;
    col3.printSorted(true);
    std::cout << "Après reconstruction - checkIndex(): " << col3.checkIndex() << " (attendu: 1)" << std::endl;
    
    std::cout << "\n=== FIN TEST 6.3 & 6.4 ===" << std::endl;

    // Test de la partie 6.5 - updateIndex()
    std::cout << "\n=== TEST PARTIE 6.5 - UPDATEINDEX ===" << std::endl;
    
    Column col4("TestUpdateIndex", ColumnType::INT);
    col4.insertValue(10);
    col4.insertValue(5);
    col4.insertValue(15);
    
    col4.sort(true);
    std::cout << "Après tri croissant - checkIndex(): " << col4.checkIndex() << std::endl;
    std::cout << "Ordre trié:" << std::endl;
    col4.printSorted(true);
    
    std::cout << "\nInsertion d'une nouvelle valeur (3)..." << std::endl;
    col4.insertValue(3);
    std::cout << "Après insertion - checkIndex(): " << col4.checkIndex() << " (index invalide)" << std::endl;
    
    std::cout << "\nAppel de updateIndex()..." << std::endl;
    col4.updateIndex();
    std::cout << "Après updateIndex() - checkIndex(): " << col4.checkIndex() << std::endl;
    std::cout << "Ordre trié mis à jour:" << std::endl;
    col4.printSorted(true);
    
    std::cout << "\n=== FIN TEST 6.5 ===" << std::endl;

    // Test de la partie 6.6 - searchValue()
    std::cout << "\n=== TEST PARTIE 6.6 - SEARCHVALUE ===" << std::endl;
    
    Column col5("TestSearch", ColumnType::INT);
    col5.insertValue(10);
    col5.insertValue(5);
    col5.insertValue(15);
    col5.insertValue(20);
    col5.insertValue(3);
    
    std::cout << "Recherche avant tri - searchValue(10): " << col5.searchValue(10) << " (attendu: -1, pas trié)" << std::endl;
    
    col5.sort(true);
    std::cout << "\nAprès tri:" << std::endl;
    col5.printSorted(true);
    
    std::cout << "\nRecherches dichotomiques:" << std::endl;
    std::cout << "searchValue(10): " << col5.searchValue(10) << " (attendu: 1, trouvé)" << std::endl;
    std::cout << "searchValue(5): " << col5.searchValue(5) << " (attendu: 1, trouvé)" << std::endl;
    std::cout << "searchValue(3): " << col5.searchValue(3) << " (attendu: 1, trouvé)" << std::endl;
    std::cout << "searchValue(7): " << col5.searchValue(7) << " (attendu: 0, non trouvé)" << std::endl;
    std::cout << "searchValue(25): " << col5.searchValue(25) << " (attendu: 0, non trouvé)" << std::endl;
    
    std::cout << "\n=== FIN TEST 6.6 ===" << std::endl;

    std::cout << "\n=== DEBUT TEST 7 ===" << std::endl;

    Column col7("TestExistAndReplace", ColumnType::STRING);
    col7.insertValue(std::string("test1"));
    col7.insertValue(std::string("test2"));
    col7.insertValue(std::string("test3"));

    Column col8("AnotherColumn", ColumnType::FLOAT);
    col8.insertValue(1.5f);
    col8.insertValue(2.5f);
    col8.insertValue(3.5f);
    col8.insertValue(4.5f);

    CDataframe df7({col7, col8, c4});
    df7.display();
    df7.renameCol(&col7, "RenamedColumn");
    df7.display();

    // =====================
    // Tests Partie 8 (int-only)
    // =====================
    std::cout << "\n=== TEST PARTIE 8 - DATAFRAME (int-only) ===" << std::endl;

    // Création via types et renommage des colonnes
    std::vector<ColumnType> types = {ColumnType::INT, ColumnType::INT, ColumnType::INT};
    CDataframe df(types);
    df.setColumnNames({"A", "B", "C"});

    // Insertion de lignes
    df.insertRow(std::vector<ColumnValue>{1, 2, 3});
    df.insertRow(std::vector<ColumnValue>{4, 5, 6});
    df.insertRow(std::vector<ColumnValue>{7, 8, 9});

    // Affichages
    std::cout << "\nHeader:" << std::endl;
    df.printHeader();
    std::cout << "\nPrint complet:" << std::endl;
    df.display();
    std::cout << "\nHead:" << std::endl;
    df.head();
    std::cout << "\nTail:" << std::endl;
    df.tail();
    std::cout << "\nInfo:" << std::endl;
    df.info();

    // Suppression de colonne par nom
    df.deleteColumn("B");
    std::cout << "\nAprès deleteColumn('B'), nb colonnes = " << df.getColumnsCount() << std::endl;
    df.printHeader();

    // Round-trip CSV (sur un autre DF pour garder 3 colonnes)
    CDataframe df2(types);
    df2.setColumnNames({"X", "Y", "Z"});
    df2.insertRow(std::vector<ColumnValue>{10, 20, 30});
    df2.insertRow(std::vector<ColumnValue>{40, 50, 60});

    std::cout << "\nSauvegarde CSV -> df_test.csv" << std::endl;
    df2.saveToCSV("df_test.csv");

    std::cout << "Chargement CSV depuis df_test.csv" << std::endl;
    auto df3 = CDataframe::loadFromCSV("df_test.csv", types);
    df3->head();
    std::cout << "Rows chargées: " << df3->getRowsCount() << ", Colonnes: " << df3->getColumnsCount() << std::endl;

    std::cout << "\n=== FIN TEST PARTIE 8 ===" << std::endl;

    Column col9("TestReplaceValue", ColumnType::INT);
    std::cout << "Insertion de 3.55f dans col9 (type INT): "
              << (col9.insertValue(3.55f) ? "Succès" : "Échec") << std::endl;

    col9.display();

    return 0;
}
