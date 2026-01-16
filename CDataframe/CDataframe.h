#pragma once

#include <vector>
#include <optional>
#include <memory>
#include <string>
#include <fstream>

#include "../Column/Column.h"

/**
 * @class CDataframe
 * @brief Lightweight dataframe-like structure built on top of Columns.
 *
 * The CDataframe class stores a set of columns (shared ownership) and provides:
 * - display helpers (full display, head, tail, selected columns),
 * - column/row insertion and deletion,
 * - simple statistics (counts and comparisons),
 * - CSV import/export utilities,
 * - helper accessors to retrieve columns by name or index.
 *
 * @note Columns are stored as std::shared_ptr<Column> to allow shared ownership
 *       and cheap copies of the dataframe container.
 */
class CDataframe
{
private:
    /**
     * @brief Container of columns composing the dataframe.
     *
     * Each column is stored using shared ownership.
     */
    std::vector<std::shared_ptr<Column>> columns;

    // ===== DISPLAY =====

    /**
     * @brief Internal print routine used by display/head/tail/displayCol.
     *
     * This method prints the dataframe with optional row slicing and optional
     * column subset.
     *
     * @param firstRowOpt Optional first row parameter (interpretation depends on caller).
     * @param lastRowOpt Optional last row parameter (interpretation depends on caller).
     * @param colOpt Optional pointer to a subset of columns to print.
     *               If nullptr, prints the dataframe's internal columns.
     */
    void print(std::optional<int> firstRowOpt,
               std::optional<int> lastRowOpt,
               const std::vector<Column>* colOpt = nullptr);

public:
    // ===== CONSTRUCTORS / DESTRUCTOR =====

    /**
     * @brief Default constructor.
     *
     * Creates an empty dataframe (no columns).
     */
    CDataframe();

    /**
     * @brief Construct a dataframe from a list of column types.
     *
     * Typically used to create columns according to types (implementation-defined).
     *
     * @param types Vector of ColumnType describing each column type.
     */
    CDataframe(const std::vector<ColumnType>& types);

    /**
     * @brief Construct a dataframe from an existing set of columns.
     *
     * @param cols Vector of Column objects.
     */
    CDataframe(const std::vector<Column>& cols);

    /**
     * @brief Construct a dataframe from an initializer list of columns.
     *
     * @param cols Initializer list of Column objects.
     */
    CDataframe(std::initializer_list<Column> cols);

    /**
     * @brief Destructor.
     */
    ~CDataframe();

    // ===== DISPLAY =====

    /**
     * @brief Display the whole dataframe.
     */
    void display();

    /**
     * @brief Display the first rows of the dataframe.
     *
     * @param row Optional number of rows to display. If not provided, implementation may choose a default.
     */
    void head(std::optional<int> row = std::nullopt);

    /**
     * @brief Display the last rows of the dataframe.
     *
     * @param row Optional number of rows to display. If not provided, implementation may choose a default.
     */
    void tail(std::optional<int> row = std::nullopt);

    /**
     * @brief Display only the provided subset of columns.
     *
     * @param col Vector of columns to display.
     */
    void displayCol(const std::vector<Column>& col);

    /**
     * @brief Print the header (column names).
     */
    void printHeader() const;

    // ===== OPERATIONS =====

    /**
     * @brief Set column names in order.
     *
     * @param names Vector of column names. Must match current number of columns (implementation-defined).
     */
    void setColumnNames(const std::vector<std::string>& names);

    /**
     * @brief Insert multiple columns into the dataframe.
     *
     * @param cols Vector of raw pointers to Column (ownership/handling implementation-defined).
     * @return true if insertion succeeded, false otherwise.
     */
    bool insertColumns(const std::vector<Column*>& cols);

    /**
     * @brief Insert a single column into the dataframe.
     *
     * @param col Raw pointer to Column (ownership/handling implementation-defined).
     * @return true if insertion succeeded, false otherwise.
     */
    bool insertColumn(Column* col);

    /**
     * @brief Delete a column by its name.
     *
     * @param colName Name of the column to delete.
     * @return true if deleted, false if not found or failed.
     */
    bool deleteColumn(const std::string& colName);

    /**
     * @brief Insert multiple rows into the dataframe.
     *
     * Each row is represented as a vector of ColumnValue.
     *
     * @param rows Vector of rows.
     * @return true if insertion succeeded, false otherwise.
     */
    bool insertRows(const std::vector<std::vector<ColumnValue>>& rows);

    /**
     * @brief Insert a single row into the dataframe.
     *
     * @param values Row values (one per column).
     * @return true if insertion succeeded, false otherwise.
     */
    bool insertRow(const std::vector<ColumnValue>& values);

    /**
     * @brief Delete a row by index.
     *
     * @param idx Zero-based row index.
     * @return true if deletion succeeded, false otherwise.
     */
    bool deleteRow(const int idx);

    /**
     * @brief Rename a column.
     *
     * @param col Pointer to the column to rename.
     * @param newName New name.
     * @return true if renamed, false otherwise.
     */
    bool renameCol(Column* col, const std::string& newName);

    /**
     * @brief Check whether a value exists somewhere in the dataframe.
     *
     * @param val Value to search.
     * @return true if found, false otherwise.
     */
    bool exist(const int val);

    /**
     * @brief Replace a value within a specified column at a given row index.
     *
     * @param col Column reference (used to identify target column).
     * @param index Row index to replace.
     * @param newVal New integer value.
     * @return true if replacement succeeded, false otherwise.
     */
    bool replaceValue(const Column& col, const int index, const int newVal);

    // ===== STATISTICS & INFO =====

    /**
     * @brief Get number of columns.
     * @return Number of columns.
     */
    size_t getColumnsCount() const;

    /**
     * @brief Get number of rows.
     * @return Number of rows.
     */
    size_t getRowsCount() const;

    /**
     * @brief Get number of rows (legacy/int API).
     * @return Number of rows.
     */
    int numberOfRows();

    /**
     * @brief Get number of columns (legacy/int API).
     * @return Number of columns.
     */
    int numberOfCols();

    /**
     * @brief Count cells equal to a given integer.
     *
     * @param x Value to compare.
     * @return Number of matching cells.
     */
    int numberOfCellsEqualTo(int x);

    /**
     * @brief Count cells strictly greater than a given integer.
     *
     * @param x Value to compare.
     * @return Number of cells greater than x.
     */
    int numberOfCellsGreaterThan(int x);

    /**
     * @brief Count cells strictly lower than a given integer.
     *
     * @param x Value to compare.
     * @return Number of cells lower than x.
     */
    int numberOfCellsLowerThan(int x);

    /**
     * @brief Print dataframe information (implementation-defined).
     */
    void info() const;

    // ===== CSV METHODS =====

    /**
     * @brief Load a dataframe from a CSV file with explicit column types.
     *
     * @param filename Path to the CSV file.
     * @param types Column types in order.
     * @return Unique pointer owning the created dataframe.
     */
    static std::unique_ptr<CDataframe> loadFromCSV(
        const std::string& filename,
        const std::vector<ColumnType>& types
    );

    /**
     * @brief Load a dataframe from a CSV file by inferring column types automatically.
     *
     * @param filename Path to the CSV file.
     * @return Unique pointer owning the created dataframe.
     */
    static std::unique_ptr<CDataframe> loadFromCSVAuto(const std::string& filename);

    /**
     * @brief Save the dataframe to a CSV file.
     *
     * @param filename Output CSV file path.
     */
    void saveToCSV(const std::string& filename) const;

    // ===== HELPERS =====

    /**
     * @brief Return the size of the biggest column.
     *
     * This can be interpreted as the dataframe row count.
     *
     * @return Maximum size among columns.
     */
    int sizeBiggestCol();

    /**
     * @brief Retrieve a column by its name.
     *
     * @param name Column name.
     * @return Shared pointer to the column, or nullptr if not found.
     */
    std::shared_ptr<Column> getColumnByName(const std::string& name);

    /**
     * @brief Retrieve a column by its index.
     *
     * @param index Zero-based column index.
     * @return Shared pointer to the column, or nullptr if out of range.
     */
    std::shared_ptr<Column> getColumnByIndex(size_t index);
};
