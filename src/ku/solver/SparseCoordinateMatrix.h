#pragma once
#include "../Sudo.h"

#include <unordered_map>

/** The data contained in a column of a sparse coordinate matrix
 */
struct SparseCoordinateColumn {
  /// Whether the column is primary
  bool isPrimary = true;
  /// A map of all the non-negative elements of the column. Key is the sparse matrix index, value is the data stored in
  /// that index
  std::unordered_map<int32_t, int32_t> elements = {};
};

/** A data structure for sparse matrices that uses coordinate format and column-major order. All elements of the matrix
 * are initialized with the value -1. By default, all columns of the matrix are primary unless manually modified.
 */
class SparseCoordinateMatrix {
public:
  /** Constructor. Creates an empty sparse coordinate matrix. The elements can be set individually with setCell()
   * @param totalRows The amount of rows the sparse matrix has
   * @param totalColumns The amount of columns the sparse matrix has
   */
  SparseCoordinateMatrix(int32_t totalRows, int32_t totalColumns);

  /** Constructor. Creates a sparse coordinate matrix according to a given
   */
  SparseCoordinateMatrix(const std::vector<std::vector<int32_t>>& matrix);

  /** Retrieves the amount of columns that this matrix supports
   * @return The amount of columns
   */
  int32_t getColumnsAmount() const;

  /** Retrieves the amount of rows that this matrix supports
   * @return The amount of rows
   */
  int32_t getRowsAmount() const;

  /** Retrieves the amount of valid (non-negative) elements
   * @return The amount of valid elements
   */
  int32_t getValidElementsAmount() const;

  /** Sets a particular column to be secondary
   * @return The amount of rows
   */
  void setColumnSecondary(int32_t columnIndex);

  /** Retrieves whether a specified column is primary
   * @param columnIndex The column index
   * @return Whether the column is primary
   */
  bool isColumnPrimary(int32_t columnIndex) const;

  /** Sets data into a specific location in the matrix. Setting a negative value will mark the cell as empty
   * @param rowIndex The row index of the insertion location
   * @param columnIndex The column index of the insertion location
   * @param data The data for the cell.
   * @return Whether insertion was successful
   */
  bool setCell(int32_t rowIndex, int32_t columnIndex, int32_t data);

  /** Gets data from a specific location in the matrix. Returns -1 if the location is outside the matrix
   * @param rowIndex The row index of the insertion location
   * @param columnIndex The column index of the insertion location
   * @return The data stored in the cell position.
   */
  int32_t getCell(int32_t rowIndex, int32_t columnIndex) const;

  /** Computes whether the matrix might be solvable by Algorithm X. This is the case when the matrix contains only
   * secondary columns, or when any primary column has no elements
   * @return Whether the matrix might be solvable
   */
  bool isSolvableByAlgorithmX() const;

  /** Reorders the matrix columns according to the new column index provided
   * @param permutation The new order for the columns
   * @return Whether reordering can be performed
   */
  bool reorderColumns(const std::vector<int32_t>& permutation);

  /** Reorders the matrix columns: first primary columns, then secondary columns. Within the two categories, columns
   * with less valid elements are placed first.
   */
  void sortPrimaryAndSecondaryColumns();

private:
  /** Resets the sparse matrix. I.e. rows and columns are set to 0 and the columns vector is cleared.
   */
  void reset();

private:
  /// The matrix. Stores only non-zero elements. Has a vector for every column
  std::vector<SparseCoordinateColumn> columns;

  /// The maximum amount of columns that the matrix considers
  int32_t columnsAmount = 0;

  /// The maximum amount of rows that the matrix considers
  int32_t rowsAmount = 0;
};