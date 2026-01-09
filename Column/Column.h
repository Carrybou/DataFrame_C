#ifndef COLUMNS_H
#define COLUMNS_H

#include <vector>
#include <string>
#include <optional>
#include <algorithm>
#include <numeric>

const size_t REALLOC_SIZE = 256;

/**
 * @brief Column class for storing integer values
 */
class Column {
private:
    std::string title;
    std::vector<int> data;
    std::vector<size_t> index;
    bool validIndex;
    bool sortAscending;

    /**
     * @brief Compare two values
     * @param a First value
     * @param b Second value
     * @return -1 if a < b, 0 if a == b, 1 if a > b
     */
    int compareValues(int a, int b) const;

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
     * @brief : add a new value (int or null) to a column
     * @param value : the value to be added (use std::nullopt for null)
     * @return : true if the value is added, false otherwise
     */
    bool insertValue(std::optional<int> value);

    /**
     * @brief : remove a value (int or null) to a given index
     * @param index : the index of the value to remove
     * @return : true if the value is added, false otherwise
     */
    bool removeValue(const int index);

    /**
     * @brief Retrieves the integer value (or null) at a specified index
     * @param index The zero-based index position
     * @return The optional<int> at the specified index, or std::nullopt if index out of range
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
     * @brief Sets a new title/name for the column
     * @param newName The new name to assign to the column
     * @return True if the name was successfully set, false otherwise
     */
    bool setName(const std::string newName);

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

    /**
     * @brief Sort a column according to a given order
     * @param ascending : true for ascending, false for descending
     */
    void sort(bool ascending = true);

    /**
     * @brief Display the contents of a column in sorted order
     * @param ascending: true for ascending, false for descending
     */
    void printSorted(bool ascending = true);

    /**
     * @brief Remove the index of a column
     */
    void eraseIndex();

    /**
     * @brief Check if an index is correct
     * @return: -1: index not existing,
     *           0: the index exists but invalid,
     *           1: the index is correct
     */
    int checkIndex() const;

    /**
     * @brief Update the index
     */
    void updateIndex();

    /**
     * @brief Test if a value exists in a column
     * @param val: The value to search for
     * @return: -1: column not sorted,
     *           0: value not found
     *           1: value found
     */
    int searchValue(int val) const;

    /**
     * @brief Resarch if a value exist in the column
     * @param value The value to search for
     * @return true if the value exist, false otherwise
     */
    bool exist(const int value);
};

#endif