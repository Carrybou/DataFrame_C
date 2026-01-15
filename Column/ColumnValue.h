#include <any>

enum class ColumnType {
    NULLVAL = 1,
    UINT,
    INT,
    USHORT,
    SHORT,
    ULONG,
    LONG,
    UCHAR,
    CHAR,
    FLOAT,
    DOUBLE,
    STRING,
    OBJECT
};

using ColumnValue = std::variant<
    std::monostate, // Pour NULL
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
    std::any // Pour les objets quelconques
>;
