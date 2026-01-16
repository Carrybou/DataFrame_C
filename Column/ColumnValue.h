#include <any>

/**
 * @enum ColumnType
 * @brief Supported logical types for a Column.
 *
 * This enumeration describes the type of data a column is intended to store.
 * It is typically used to validate insertions, choose comparisons, and format output.
 */
enum class ColumnType {
    NULLVAL = 1, /**< Null / missing values */
    UINT,        /**< Unsigned 32-bit integer */
    INT,         /**< Signed 32-bit integer */
    USHORT,      /**< Unsigned 16-bit integer */
    SHORT,       /**< Signed 16-bit integer */
    ULONG,       /**< Unsigned 64-bit integer */
    LONG,        /**< Signed 64-bit integer */
    UCHAR,       /**< Unsigned 8-bit integer */
    CHAR,        /**< Signed 8-bit integer */
    FLOAT,       /**< 32-bit floating point */
    DOUBLE,      /**< 64-bit floating point */
    STRING,      /**< std::string */
    OBJECT       /**< Arbitrary object (std::any) */
};

/**
 * @typedef ColumnValue
 * @brief Variant holding any value that can be stored in a column.
 *
 * `ColumnValue` is a tagged union based on `std::variant`.
 * It supports:
 * - a NULL value via `std::monostate`
 * - several numeric scalar types
 * - `std::string`
 * - a generic payload via `std::any`
 *
 * @note `std::any` is not order-comparable. If you sort/compare ColumnValue instances,
 *       you must define custom comparison logic that does not rely on `<`/`>` for `std::any`.
 */
using ColumnValue = std::variant<
    std::monostate, // NULL / missing value
    uint32_t,
    int32_t,
    uint16_t,
    int16_t,
    uint64_t,
    int64_t,
    uint8_t,
    int8_t,
    float,
    double,
    std::string,
    std::any // Arbitrary objects
>;
