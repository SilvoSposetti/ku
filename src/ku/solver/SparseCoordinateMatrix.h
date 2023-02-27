#pragma once
#include "../utilities/Utilities.h"

/** The data contained in the cell of a sparse coordinate matrix
 */
struct SparseCooordinateCell {
  int32_t matrixRow;
  int32_t value;
};

/** A data structure for sparse matrices that uses coordinate format and column-major order.
 */
class SparseCooordinateMatrix {
public:
  /** Constructor
   * @param totalRows The amount of rows the sparse matrix has
   * @param totalColumns The amount of columns the sparse matrix has
   */
  SparseCooordinateMatrix(int32_t totalRows, int32_t totalColumns);

  /** Sets data into a specific location in the matrix
   * @param rowIndex The row index of the insertion location
   * @param columnIndex The column index of the insertion location
   * @return Whether insertion was successful
   */
  bool setData(int32_t rowIndex, int32_t columnIndex, int32_t data);

  /** Gets data from a specific location in the matrix. Returns 0 if the
   * @param rowIndex The row index of the insertion location
   * @param columnIndex The column index of the insertion location
   * @return The value storedf in the cell
   */
  int32_t getData(int32_t rowIndex, int32_t columnIndex) const;

  /** Reorders the matrix columns according to the new column index provided
   * @param permutation The new order for the columns
   * @return Whether reordering can be performed
   */
  bool reorderColumns(const std::vector<int32_t>& permutation);

private:
  /// The matrix. Stores only non-zero elements. Has a vector for every column
  std::vector<std::vector<SparseCooordinateCell>> matrix;

  /// The maximum amount of columns that the matrix considers
  int32_t columns = 0;

  /// The maximum amount of rows that the matrix considers
  int32_t rows = 0;
};