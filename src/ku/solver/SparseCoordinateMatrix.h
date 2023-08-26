#pragma once
#include "../Sudo.h"

#include <memory>
#include <set>
#include <unordered_set>

/** A data structure for sparse matrices that uses coordinate format and column-major order. All elements of the matrix
 * are initialized with the value -1. By default, all columns of the matrix are primary unless manually modified.
 */
class SparseCoordinateMatrix {
public:
  /** Constructor. Creates a sparse coordinate matrix according to a given binary matrix and the data associated with
   * each row.
   * @param matrix The binary matrix in row-major order
   * @param secondaryColumnsIndices The indices of the secondary columns
   * @param placeSecondaryColumnsAtEnd Whether secondary columns should be placed at the end
   * @param rowsDataVector The data for each row. If empty, the default value of -1 is used for every row
   */
  SparseCoordinateMatrix(const std::vector<std::vector<bool>>& classicMatrix,
                         const std::unordered_set<int32_t>& secondaryColumnsIndices = {},
                         bool placeSecondaryColumnsAtEnd = true,
                         const std::vector<int32_t>& rowsDataVector = {});

  /** Prepares the matrix for Algorithm X. Puts primary columns at the beginning of the matrix and secondary columns at
   * the end.
   */
  void preprocess();

  /** Retrieves the amount of columns that this matrix supports
   * @return The amount of columns
   */
  int32_t getColumnsAmount() const;

  /** Retrieves the amount of rows that this matrix supports
   * @return The amount of rows
   */
  int32_t getRowsAmount() const;

  /** Retrieves the amount of valid (set) elements
   * @return The amount of valid elements
   */
  int32_t getValidElementsAmount() const;

  /** Retrieves whether a specified column is primary
   * @param columnIndex The column index
   * @return Whether the column is primary
   */
  bool isColumnPrimary(int32_t columnIndex) const;

  /** Returns whether a specific cell is set or not. Returns false if the location is outside of the matrix.
   * @param rowIndex The row index of the insertion location
   * @param columnIndex The column index of the insertion location
   * @return The data stored in the cell position.
   */
  bool isCellSet(int32_t rowIndex, int32_t columnIndex) const;

  /** Sets the data of a specific row of the matrix
   * @param rowIndex The row index of the insertion location
   * @param data The column index of the insertion location
   * @return The data of the row if successful, otherwise -1
   */
  int32_t getRowData(int32_t rowIndex) const;

  /** Computes whether the matrix might be solvable by Algorithm X. This is the case when the matrix contains only
   * secondary columns, or when any primary column has no elements
   * @return Whether the matrix might be solvable
   */
  bool isSolvableByAlgorithmX() const;

  /** Retrieves the amount of valid cells within a column
   * @param columnIndex The index of the column
   * @return The amount of non-zero elements that a column contains
   */
  int32_t getColumnValidCellsAmount(int32_t columnIndex) const;

private:
  /** Helper to compute whether a certain column index is valid
   * @param columnIndex The column index
   * @return Whether the index is valid
   */
  bool isValidColumnIndex(int32_t columnIndex) const;

  /** Helper to compute whether a certain row index is valid
   * @param rowIndex The row index
   * @return Whether the index is valid
   */
  bool isValidRowIndex(int32_t rowIndex) const;

private:
  /// The maximum amount of rows that the matrix considers
  std::size_t rowsAmount = 0;

  /// The maximum amount of columns that the matrix considers
  std::size_t columnsAmount = 0;

  /// The binary matrix, arranged by column
  std::vector<std::vector<bool>> matrix;

  /// Sorted sets of all the non-zero indices of elements for every row
  std::vector<std::set<int32_t>> rowsElements;

  /// The data stored within a row
  std::vector<int32_t> rowsData;

  /// The Whether the columns are primary (not secondary)
  std::vector<bool> areColumnsPrimary;

  static constexpr int32_t invalidRowValue = -1;
};