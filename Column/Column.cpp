// ========================= Column.cpp =========================
#include <iostream>
#include <optional>
#include <variant>
#include <numeric>
#include <type_traits>
#include <algorithm>
#include <any>
#include <limits>
#include <cstdint>

#include "Column.h"

Column::Column(const std::string& colName, ColumnType type)
{
    this->title = colName;
    this->columnType = type;
    this->data = std::vector<std::optional<ColumnValue>>();
    this->data.reserve(REALLOC_SIZE);
    this->index = std::vector<size_t>();
    this->validIndex = false;
    this->sortAscending = true;
}

// column can store all types of ColumnValue
bool Column::insertValue(std::optional<ColumnValue> value)
{
    // check for each enum type
    if (value.has_value()) {
        switch (this->columnType) {
            case ColumnType::NULLVAL:
                if (!std::holds_alternative<std::monostate>(value.value()))
                    return false;
                break;
            case ColumnType::UINT:
                if (!std::holds_alternative<uint32_t>(value.value()))
                    return false;
                break;
            case ColumnType::INT:
                if (!std::holds_alternative<int32_t>(value.value()))
                    return false;
                break;
            case ColumnType::USHORT:
                if (!std::holds_alternative<uint16_t>(value.value()))
                    return false;
                break;
            case ColumnType::SHORT:
                if (!std::holds_alternative<int16_t>(value.value()))
                    return false;
                break;
            case ColumnType::ULONG:
                if (!std::holds_alternative<uint64_t>(value.value()))
                    return false;
                break;
            case ColumnType::LONG:
                if (!std::holds_alternative<int64_t>(value.value()))
                    return false;
                break;
            case ColumnType::UCHAR:
                if (!std::holds_alternative<uint8_t>(value.value()))
                    return false;
                break;
            case ColumnType::CHAR:
                if (!std::holds_alternative<int8_t>(value.value()))
                    return false;
                break;
            case ColumnType::FLOAT:
                if (!std::holds_alternative<float>(value.value()))
                    return false;
                break;
            case ColumnType::DOUBLE:
                if (!std::holds_alternative<double>(value.value()))
                    return false;
                break;
            case ColumnType::STRING:
                if (!std::holds_alternative<std::string>(value.value()))
                    return false;
                break;
            case ColumnType::OBJECT:
                if (!std::holds_alternative<std::any>(value.value()))
                    return false;
                break;
            default:
                return false;
        }
    }

    data.push_back(value);
    validIndex = false;
    return true;
}

bool Column::removeValue(const int index)
{
    if (index < 0 || static_cast<size_t>(index) >= data.size())
        return false;

    data.erase(data.begin() + index);
    validIndex = false;
    return true;
}

std::optional<ColumnValue> Column::getValueAt(int index) const
{
    if (index < 0 || static_cast<size_t>(index) >= data.size())
        return std::nullopt;

    return data[index];
}

int Column::getSize() const
{
    return static_cast<int>(this->data.size());
}

std::string Column::getName() const
{
    return this->title;
}

bool Column::setName(const std::string newValue)
{
    if (newValue.empty())
        return false;

    this->title = newValue;
    return true;
}

void Column::display() const
{
    for (int i = 0; i < this->getSize(); i++)
    {
        std::cout << "[" << i << "] ";
        if (this->data[i].has_value())
            std::cout << this->valueToString(static_cast<size_t>(i));
        else
            std::cout << "NULL";
        std::cout << "\n";
    }
}

/* -------------------- comparisons -------------------- */

static int compareColumnValues(const ColumnValue& a, const ColumnValue& b)
{
    return std::visit(
        [](auto&& va, auto&& vb) -> int {
            using A = std::decay_t<decltype(va)>;
            using B = std::decay_t<decltype(vb)>;

            // NULL handling
            if constexpr (std::is_same_v<A, std::monostate> && std::is_same_v<B, std::monostate>) return 0;
            if constexpr (std::is_same_v<A, std::monostate>) return -1;
            if constexpr (std::is_same_v<B, std::monostate>) return 1;

            // string vs string
            if constexpr (std::is_same_v<A, std::string> && std::is_same_v<B, std::string>) {
                if (va < vb) return -1;
                if (va > vb) return 1;
                return 0;
            }

            // any is not comparable
            if constexpr (std::is_same_v<A, std::any> || std::is_same_v<B, std::any>) {
                return 0;
            }

            // numeric vs numeric
            if constexpr (std::is_arithmetic_v<A> && std::is_arithmetic_v<B>) {
                long double da = static_cast<long double>(va);
                long double db = static_cast<long double>(vb);
                if (da < db) return -1;
                if (da > db) return 1;
                return 0;
            }

            // incompatible
            return 0;
        },
        a, b
    );
}

int Column::occurence(const ColumnValue& value) const
{
    if (this->data.empty()) return 0;

    int cnt = 0;
    for (int i = 0; i < this->getSize(); i++)
        if (this->data[i].has_value() && compareColumnValues(this->data[i].value(), value) == 0)
            cnt++;

    return cnt;
}

int Column::numberGreaterThan(const ColumnValue& value) const
{
    if (this->data.empty()) return 0;
    if (this->columnType == ColumnType::STRING || this->columnType == ColumnType::OBJECT) return 0;

    int cnt = 0;
    for (int i = 0; i < this->getSize(); i++)
        if (this->data[i].has_value() && compareColumnValues(this->data[i].value(), value) > 0)
            cnt++;

    return cnt;
}

int Column::numberLowerThan(const ColumnValue& value) const
{
    if (this->data.empty()) return 0;
    if (this->columnType == ColumnType::STRING || this->columnType == ColumnType::OBJECT) return 0;

    int cnt = 0;
    for (int i = 0; i < this->getSize(); i++)
        if (this->data[i].has_value() && compareColumnValues(this->data[i].value(), value) < 0)
            cnt++;

    return cnt;
}

int Column::compareValues(const ColumnValue& a, const ColumnValue& b) const
{
    return compareColumnValues(a, b);
}

void Column::sort(bool ascending)
{
    if (this->index.empty() || this->index.size() != this->data.size()) {
        this->index.resize(this->data.size());
        std::iota(this->index.begin(), this->index.end(), 0);
    }

    std::sort(this->index.begin(), this->index.end(),
        [this, ascending](size_t a, size_t b) {
            const bool aNull = !this->data[a].has_value();
            const bool bNull = !this->data[b].has_value();

            // NULLs last if ascending, first if descending
            if (aNull || bNull) {
                if (aNull && bNull) return false;
                if (ascending) return !aNull && bNull;
                return aNull && !bNull;
            }

            int cmp = compareColumnValues(this->data[a].value(), this->data[b].value());
            return ascending ? (cmp < 0) : (cmp > 0);
        });

    this->validIndex = true;
    this->sortAscending = ascending;
}

void Column::printSorted(bool ascending)
{
    if (!this->validIndex || this->sortAscending != ascending)
        this->sort(ascending);

    for (size_t i = 0; i < this->index.size(); i++) {
        size_t idx = this->index[i];
        std::cout << "[" << idx << "] ";
        if (this->data[idx].has_value())
            std::cout << this->valueToString(idx);
        else
            std::cout << "NULL";
        std::cout << "\n";
    }
}

void Column::eraseIndex()
{
    this->validIndex = false;
}

int Column::checkIndex() const
{
    if (this->index.empty()) return -1;
    if (!this->validIndex) return 0;
    return 1;
}

void Column::updateIndex()
{
    if (this->index.empty()) this->sort(true);
    else this->sort(this->sortAscending);
}

int Column::searchValue(const ColumnValue& val) const
{
    if (!this->validIndex) return -1;

    size_t left = 0;
    size_t right = this->index.size();

    while (left < right) {
        size_t mid = left + (right - left) / 2;
        size_t idx = this->index[mid];

        if (!this->data[idx].has_value()) {
            if (this->sortAscending) right = mid;
            else left = mid + 1;
            continue;
        }

        int cmp = compareColumnValues(this->data[idx].value(), val);
        if (cmp == 0) return 1;
        if (cmp < 0) left = mid + 1;
        else right = mid;
    }

    return 0;
}

bool Column::exist(const ColumnValue& value)
{
    if (!this->validIndex) {
        for (const auto& cell : data)
            if (cell.has_value() && compareColumnValues(cell.value(), value) == 0)
                return true;
        return false;
    }
    return this->searchValue(value) == 1;
}

bool Column::accessReplaceValue(int row, std::optional<ColumnValue> newValue)
{
    if (row < 0 || static_cast<size_t>(row) >= this->data.size())
        return false;

    this->data[row] = newValue;
    this->validIndex = false;
    return true;
}

std::string Column::valueToString(size_t i) const
{
    // Petit helper: conversion en string (pour colonne STRING)
    auto toString = [](const ColumnValue& x) -> std::string {
        return std::visit([](auto&& arg) -> std::string {
            using T = std::decay_t<decltype(arg)>;

            if constexpr (std::is_same_v<T, std::monostate>) {
                return "NULL";
            } else if constexpr (std::is_same_v<T, std::string>) {
                return arg;
            } else if constexpr (std::is_same_v<T, std::any>) {
                return "[object]";
            } else if constexpr (std::is_same_v<T, std::uint8_t> || std::is_same_v<T, std::int8_t>) {
                // éviter l'affichage en caractère
                return std::to_string(static_cast<int>(arg));
            } else if constexpr (std::is_arithmetic_v<T>) {
                return std::to_string(arg);
            } else {
                return "[unsupported]";
            }
        }, x);
    };

    if (!data[i].has_value()) return "NULL";

    return std::visit([](auto&& arg) -> std::string {
        using T = std::decay_t<decltype(arg)>;

        if constexpr (std::is_same_v<T, std::monostate>) {
            return "NULL";
        } else if constexpr (std::is_same_v<T, std::string>) {
            return arg;
        } else if constexpr (std::is_same_v<T, std::any>) {
            return "[object]";
        } else if constexpr (std::is_same_v<T, std::uint8_t> || std::is_same_v<T, std::int8_t>) {
            return std::to_string(static_cast<int>(arg));
        } else {
            return std::to_string(arg);
        }
    }, data[i].value());
}

bool Column::insertValueAuto(const ColumnValue& v)
{
    // NULL accepté partout
    if (std::holds_alternative<std::monostate>(v))
        return insertValue(std::nullopt);

    auto toString = [](const ColumnValue& x) -> std::string {
        return std::visit([](auto&& arg) -> std::string {
            using T = std::decay_t<decltype(arg)>;

            if constexpr (std::is_same_v<T, std::monostate>) {
                return "NULL";
            } else if constexpr (std::is_same_v<T, std::string>) {
                return arg;
            } else if constexpr (std::is_same_v<T, std::any>) {
                return "[object]";
            } else if constexpr (std::is_same_v<T, std::uint8_t> || std::is_same_v<T, std::int8_t>) {
                return std::to_string(static_cast<int>(arg));
            } else if constexpr (std::is_arithmetic_v<T>) {
                return std::to_string(arg);
            } else {
                return "[unsupported]";
            }
        }, x);
    };


    auto toNumber = [](const ColumnValue& x) -> std::optional<long double> {
        return std::visit([](auto&& arg) -> std::optional<long double> {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, std::monostate> || std::is_same_v<T, std::string> || std::is_same_v<T, std::any>)
                return std::nullopt;
            if constexpr (std::is_arithmetic_v<T>)
                return static_cast<long double>(arg);
            return std::nullopt;
        }, x);
    };

    switch (this->columnType) {
        case ColumnType::NULLVAL:
            return false;

        case ColumnType::OBJECT: {
            if (std::holds_alternative<std::any>(v))
                return insertValue(std::optional<ColumnValue>(v));
            return insertValue(std::optional<ColumnValue>(ColumnValue(std::any(v))));
        }

        case ColumnType::STRING: {
            std::string s = toString(v);
            return insertValue(std::optional<ColumnValue>(ColumnValue(std::move(s))));
        }

        case ColumnType::INT: {
            auto n = toNumber(v); if (!n) return false;
            return insertValue(std::optional<ColumnValue>(ColumnValue(static_cast<int32_t>(*n))));
        }
        case ColumnType::UINT: {
            auto n = toNumber(v); if (!n) return false;
            return insertValue(std::optional<ColumnValue>(ColumnValue(static_cast<uint32_t>(*n))));
        }
        case ColumnType::SHORT: {
            auto n = toNumber(v); if (!n) return false;
            return insertValue(std::optional<ColumnValue>(ColumnValue(static_cast<int16_t>(*n))));
        }
        case ColumnType::USHORT: {
            auto n = toNumber(v); if (!n) return false;
            return insertValue(std::optional<ColumnValue>(ColumnValue(static_cast<uint16_t>(*n))));
        }
        case ColumnType::LONG: {
            auto n = toNumber(v); if (!n) return false;
            return insertValue(std::optional<ColumnValue>(ColumnValue(static_cast<int64_t>(*n))));
        }
        case ColumnType::ULONG: {
            auto n = toNumber(v); if (!n) return false;
            return insertValue(std::optional<ColumnValue>(ColumnValue(static_cast<uint64_t>(*n))));
        }
        case ColumnType::CHAR: {
            auto n = toNumber(v); if (!n) return false;
            return insertValue(std::optional<ColumnValue>(ColumnValue(static_cast<int8_t>(*n))));
        }
        case ColumnType::UCHAR: {
            auto n = toNumber(v); if (!n) return false;
            return insertValue(std::optional<ColumnValue>(ColumnValue(static_cast<uint8_t>(*n))));
        }
        case ColumnType::FLOAT: {
            auto n = toNumber(v); if (!n) return false;
            return insertValue(std::optional<ColumnValue>(ColumnValue(static_cast<float>(*n))));
        }
        case ColumnType::DOUBLE: {
            auto n = toNumber(v); if (!n) return false;
            return insertValue(std::optional<ColumnValue>(ColumnValue(static_cast<double>(*n))));
        }
        default:
            return false;
    }
}
