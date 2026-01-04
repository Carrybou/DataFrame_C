#ifndef COLUMNS_H
#define COLUMNS_H

#include <vector>
#include <string>

const size_t REALLOC_SIZE = 256;

/**
 * @brief Column class for storing integer values
 */
class Column {
private:
    std::string title;
    std::vector<int> data;

public:
    /**
     * @brief Constructor - create a column
     * @param columnTitle : Column title
     */
    Column(const std::string& colName);

    /**
     * @brief Destructor
     */
    //~Columns();

    /**
     * @brief : adda new value to a column
     * @param value : the value to be added
     * @return : true if the value is added, false otherwise
     */
    bool insertValue(int value);

    /**
     * @brief Retrieves the integer value at a specified index
     * @param index The zero-based index position
     * @return The integer value at the specified index
     * @throws May cause undefined behavior if index is out of bounds
     */
    std::optional<int> getValueAt(int index) const;

    /**
     * @brief Returns the number of elements currently stored in the column
     * @return The size of the data vector
     */
    int getSize() const;

    /**
     * @brief Retrieves the title/name of the column
     * @return A constant reference to the column's title string
     */
    std::string getName() const;

    /**
     * @brief Displays the column's contents to standard output
     * Prints the column title and all contained values in a formatted manner
     */
    void display() const;

    /**
     * @brief Counts the number of occurrences of a specified value in the column
     * @param value The integer value to search for
     * @return The count of how many times the value appears in the column
     */
    int occurence(int value) const;

    /**
     * @brief Counts the number of elements greater than a specified value
     * @param value The threshold value for comparison
     * @return The count of elements greater than x
     */
    int numberGreaterThan(int value) const;


    /**
     * @brief Counts the number of elements lower than a specified value
     * @param value The threshold value for comparison
     * @return The count of elements greater than x
     */
    int numberLowerThan(int value) const;
};

#endif