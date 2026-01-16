#ifndef COLUMNS_H
#define COLUMNS_H

#include "ColumnValue.h"

#include <vector>
#include <string>
#include <optional>
#include <algorithm>
#include <numeric>
#include <any>

const size_t REALLOC_SIZE = 256;


/**
 * @brief Column class for storing integer values
 */
class Column {
private:
    std::string title;
    std::vector<std::optional<ColumnValue>> data;
    std::vector<size_t> index;
    ColumnType columnType;
    bool validIndex;
    bool sortAscending;

    /**
     * @brief Compare two values
     * @param a First value
     * @param b Second value
     * @return -1 if a < b, 0 if a == b, 1 if a > b
     */
    int compareValues(const ColumnValue& a, const ColumnValue& b) const;

public:
    /**
     * @brief Constructor - create a column
     * @param type : column type
     * @param columnTitle : Column title
     */
    Column(const std::string& colName, ColumnType type);

    /**
     * @brief Destructor
     */
    //~Columns();

    /**
    * @brief: Insert a new value into a column
    * @param value : The value to insert (as ColumnValue)
    * @return: true if the value is correctly inserted, false otherwise
    */
    bool insertValue(std::optional<ColumnValue> value);

    /**
     * @brief : remove a value  to a given index
     * @param index : the index of the value to remove
     * @return : true if the value is added, false otherwise
     */
    bool removeValue(const int index);

    /**
     * @brief Retrieves the  value (or null) at a specified index
     * @param index The zero-based index position
     * @return The optional<ColumnValue> at the specified index, or std::nullopt if index out of range
     */
    std::optional<ColumnValue> getValueAt(int index) const;
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
    int occurence(const ColumnValue& value) const;

    /**
     * @brief Counts the number of elements greater than a specified value
     * @param value The threshold value for comparison
     * @return The count of elements greater than x
     */
    int numberGreaterThan(const ColumnValue& value) const;


    /**
     * @brief Counts the number of elements lower than a specified value
     * @param value The threshold value for comparison
     * @return The count of elements greater than x
     */
    int numberLowerThan(const ColumnValue& value) const;

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
    int searchValue(const ColumnValue& val) const;

    /**
     * @brief Resarch if a value exist in the column
     * @param value The value to search for
     * @return true if the value exist, false otherwise
     */
    bool exist(const ColumnValue& value);


    /**
     * @brief Access/replace the value located in a cell of the column using its row number
     * @param row The row number
     * @param newValue The new value to set
     * @return true if the value was successfully replaced, false otherwise
     */
    bool accessReplaceValue(int row, std::optional<ColumnValue> newValue);

    /**
    * @brief: Convert a column value to string
    * @param i: The index of the value to retrieve
    * @return: String representation of the value
    */
    std::string valueToString(size_t i) const;
    
    /*
    * @brief Insert a value into the column, automatically handling type conversion.
    * @param v The value to insert.
    * @return true if insertion succeeded, false otherwise.
    */
    bool insertValueAuto(const ColumnValue &v);
};


#endif