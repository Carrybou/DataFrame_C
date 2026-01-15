#ifndef COLUMNVALUE_H
#define COLUMNVALUE_H

#include <variant>
#include <string>

/**
 * @brief Enumeration for column types
 */
enum class ColumnType {
    INT,
    DOUBLE,
    STRING,
    CHAR
};

/**
 * @brief Union-like type to store different data types
 * Using std::variant to support multiple types
 */
using ColumnValue = std::variant<int, double, std::string, char>;

#endif
