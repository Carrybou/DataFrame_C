#ifndef COLUMNS_H
#define COLUMNS_H

#include <vector>
#include <string>

const size_t REALLOC_SIZE = 256;

/**
 * @brief Column class for storing integer values
 */
class Columns {
private:
    std::string title;
    std::vector<int> data;

public:
    /**
     * @brief Constructor - create a column
     * @param columnTitle : Column title
     */
    Columns(const std::string& colName);
    
    /**
     * @brief : adda new value to a column
     * @param value : the value to be added
     * @return : true if the value is added, false otherwise
     */
    void insertValue(int value);
    double getValue(int index) const;
    int getSize() const;
    std::string getName() const;
    void display() const;

    
};

#endif