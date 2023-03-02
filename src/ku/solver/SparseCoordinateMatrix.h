#pragma once
#include "../utilities/Utilities.h"

/** The data contained in the cell of a sparse coordinate matrix
 */
struct SparseCooordinateCell {
  int32_t matrixRow = 0;
  bool value = false;
};
/** The data contained in a column of a sparse coordinate matrix
 */
struct SparseCooordinateColumn {
  bool isColumnPrimary = true;
  std::vector<SparseCooordinateCell> elements;
};

/** A data structure for sparse matrices that uses coordinate format and column-major order. All elements of the matrix
 * are initialized with the value -1. By default, all columns of the matrix are primary unless manually modified.
 */
class SparseCooordinateMatrix {
public:
  /** Constructor
   * @param totalRows The amount of rows the sparse matrix has
   * @param totalColumns The amount of columns the sparse matrix has
   */
  SparseCooordinateMatrix(int32_t totalRows, int32_t totalColumns);

  /** Retrieves the amount of columns that this matrix supports
   * @return The amount of columns
   */
  int32_t getColumnsAmount() const;

  /** Retrieves the amount of rows that this matrix supports
   * @return The amount of rows
   */
  int32_t getRowsAmount() const;

  /** Sets a particular column to be secondary
   * @return The amount of rows
   */
  void setColumnSecondary(int32_t columnIndex);

  /** Retrieves whether a specified column is primary
   * @param columnIndex The column index
   * @return Whether the column is primary
   */
  bool isColumnPrimary(int32_t columnIndex) const;

  /** Sets data into a specific location in the matrix
   * @param rowIndex The row index of the insertion location
   * @param columnIndex The column index of the insertion location
   * @param value The value for the cell. I.e. whether the location contains a 0 (false) or 1 (true)
   * @return Whether insertion was successful
   */
  bool setCell(int32_t rowIndex, int32_t columnIndex, bool data);

  /** Gets data from a specific location in the matrix. Returns 0 if the
   * @param rowIndex The row index of the insertion location
   * @param columnIndex The column index of the insertion location
   * @return Whether the cell contains a 1 (true) or a zero (false)
   */
  bool getCell(int32_t rowIndex, int32_t columnIndex) const;

  /** Computes whether the matrix might be solvable by a DLX algorithm. This is the case when the matrix contains olny
   * secondary columns, or when any primary column has no elements
   * @return Whether the matrix might be solvable
   */
  bool isSolvableByDlx() const;

  /** Reorders the matrix columns according to the new column index provided
   * @param permutation The new order for the columns
   * @return Whether reordering can be performed
   */
  bool reorderColumns(const std::vector<int32_t>& permutation);

private:
  /// The matrix. Stores only non-zero elements. Has a vector for every column
  std::vector<SparseCooordinateColumn> columns;

  /// The maximum amount of columns that the matrix considers
  int32_t columnsAmount = 0;

  /// The maximum amount of rows that the matrix considers
  int32_t rowsAmount = 0;
};