#include <iostream>
#include <optional>
#include <variant>
#include <numeric>
#include <type_traits>
#include <algorithm>

#include "Column.h"

Column::Column(const std::string& colName, ColumnType type){
    this->title = colName;
    this->columnType = type;
    this->data = std::vector<std::optional<ColumnValue>>();
    this->data.reserve(REALLOC_SIZE);
    this->index = std::vector<size_t>();
    this->validIndex = false;
    this->sortAscending = true;
}

// column can store all types of ColumnValue
void Column::insertValue(std::optional<ColumnValue> value)
{
    this->data.push_back(value);
    this->validIndex = false;
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

/* -------------------- MIN FIX: comparisons -------------------- */

// Compare two ColumnValue (same variant type OR both numeric)
static int compareColumnValues(const ColumnValue& a, const ColumnValue& b)
{
    // Compare via visitor to avoid variant < > (std::any is not comparable)
    return std::visit(
        [](auto&& va, auto&& vb) -> int {
            using A = std::decay_t<decltype(va)>;
            using B = std::decay_t<decltype(vb)>;

            // NULL handling (monostate)
            if constexpr (std::is_same_v<A, std::monostate> && std::is_same_v<B, std::monostate>) {
                return 0;
            } else if constexpr (std::is_same_v<A, std::monostate>) {
                return -1;
            } else if constexpr (std::is_same_v<B, std::monostate>) {
                return 1;
            }

            // string vs string
            if constexpr (std::is_same_v<A, std::string> && std::is_same_v<B, std::string>) {
                if (va < vb) return -1;
                if (va > vb) return 1;
                return 0;
            }

            // any is not comparable -> deterministic fallback
            if constexpr (std::is_same_v<A, std::any> || std::is_same_v<B, std::any>) {
                return 0; // "equal" for sorting purposes between anys
            }

            // numeric vs numeric (all arithmetic except bool)
            if constexpr (std::is_arithmetic_v<A> && std::is_arithmetic_v<B>) {
                long double da = static_cast<long double>(va);
                long double db = static_cast<long double>(vb);
                if (da < db) return -1;
                if (da > db) return 1;
                return 0;
            }

            // incompatible types -> deterministic fallback
            return 0;
        },
        a, b
    );
}

int Column::occurence(const ColumnValue& value) const
{
    if(this->data.empty())
        return 0;

    int cnt = 0;
    for (int i = 0; i < this->getSize(); i++)
    {
        if (this->data[i].has_value() && compareColumnValues(this->data[i].value(), value) == 0)
            cnt++;
    }

    return cnt;
}

int Column::numberGreaterThan(const ColumnValue& value) const
{
    if(this->data.empty())
        return 0;

    if(this->columnType == ColumnType::STRING || this->columnType == ColumnType::OBJECT)
        return 0;

    int cnt = 0;
    for (int i = 0; i < this->getSize(); i++)
    {
        if (this->data[i].has_value() && compareColumnValues(this->data[i].value(), value) > 0)
            cnt++;
    }

    return cnt;
}

int Column::numberLowerThan(const ColumnValue& value) const
{
    if(this->data.empty())
        return 0;

    if(this->columnType == ColumnType::STRING || this->columnType == ColumnType::OBJECT)
        return 0;

    int cnt = 0;
    for (int i = 0; i < this->getSize(); i++)
    {
        if (this->data[i].has_value() && compareColumnValues(this->data[i].value(), value) < 0)
            cnt++;
    }

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

            int cmp = this->compareValues(this->data[a].value(), this->data[b].value());
            return ascending ? (cmp < 0) : (cmp > 0);
        });

    this->validIndex = true;
    this->sortAscending = ascending;
}

void Column::printSorted(bool ascending)
{
    if (!this->validIndex || this->sortAscending != ascending) {
        this->sort(ascending);
    }

    for (size_t i = 0; i < this->index.size(); i++) {
        size_t idx = this->index[i];
        std::cout << "[" << idx << "] ";
        if (this->data[idx].has_value())
            std::cout << this->valueToString(idx);
        else
            std::cout << "NULL";
        std::cout << std::endl;
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
    if (this->index.empty()) {
        this->sort(true);
    } else {
        this->sort(this->sortAscending);
    }
}

int Column::searchValue(const ColumnValue& val) const
{
    if (!this->validIndex) {
        return -1;
    }

    size_t left = 0;
    size_t right = this->index.size();

    while (left < right) {
        size_t mid = left + (right - left) / 2;
        size_t idx = this->index[mid];

        // Skip NULL
        if (!this->data[idx].has_value()) {
            // NULL zone is grouped by sort() logic, so we can shrink accordingly
            if (this->sortAscending) right = mid;
            else left = mid + 1;
            continue;
        }

        int comparison = this->compareValues(this->data[idx].value(), val);

        if (comparison == 0) {
            return 1;
        } else if (comparison < 0) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }

    return 0;
}

bool Column::exist(const ColumnValue& value)
{
    // minimal fix: if not sorted, fallback linear search
    if (!this->validIndex) {
        for (const auto& cell : data) {
            if (cell.has_value() && compareColumnValues(cell.value(), value) == 0)
                return true;
        }
        return false;
    }
    return this->searchValue(value) == 1;
}

// MIN FIX: signature must match stored type
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
    // safe: caller checks has_value(), but we keep it robust
    if (!data[i].has_value())
        return "NULL";

    return std::visit([](auto&& arg) -> std::string {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::monostate>) {
            return "NULL";
        } else if constexpr (std::is_same_v<T, std::string>) {
            return arg;
        } else if constexpr (std::is_same_v<T, std::any>) {
            return "[object]";
        } else {
            return std::to_string(arg);
        }
    }, data[i].value());
}
