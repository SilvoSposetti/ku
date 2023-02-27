#include "SparseCoordinateMatrix.h"

#include <unordered_set>

SparseCooordinateMatrix::SparseCooordinateMatrix(int32_t totalRows, int32_t totalColumns)
    : matrix(totalColumns)
    , columns(totalColumns)
    , rows(totalRows) {}

bool SparseCooordinateMatrix::setData(int32_t rowIndex, int32_t columnIndex, int32_t data) {
  // Do not insert if the cell is outside of the matrix range
  if (0 > rowIndex || rowIndex > rows - 1 || 0 > columnIndex || columnIndex > columns - 1) {
    return false;
  }

  std::vector<SparseCooordinateCell>& column = matrix[columnIndex];
  // Find the insertion index
  int32_t insertIndex = 0;
  for (auto& cell : column) {
    if (cell.matrixRow < rowIndex) {
      continue;
    } else if (cell.matrixRow == rowIndex) {
      // Cell already exists, just change its data and exit early
      cell.value = data;
      return true;
    } else {
      break;
    }
    insertIndex++;
  }

  // Add cell
  SparseCooordinateCell cell;
  cell.matrixRow = rowIndex;
  cell.value = data;
  column.insert(column.begin() + insertIndex, cell);
  return true;
}

int32_t SparseCooordinateMatrix::getData(int32_t rowIndex, int32_t columnIndex) const {
  // Return 0 when the indices are not valid
  if (0 > rowIndex || rowIndex > rows - 1 || 0 > columnIndex || columnIndex > columns - 1) {
    return 0;
  }
  // Retrieve value
  const auto& column = matrix[columnIndex];
  for (const auto& cell : column) {
    if (cell.matrixRow == rowIndex) {
      return cell.value;
    }
  }
  return 0;
}

bool SparseCooordinateMatrix::reorderColumns(const std::vector<int32_t>& permutation) {
  // Permutation's size must be the same as the amount of columns in the matrix
  if (static_cast<int32_t>(permutation.size()) != columns) {
    return false;
  }

  // All indices in the permutation must be valid indices
  if (!std::all_of(
          permutation.begin(), permutation.end(), [&](int32_t value) { return value >= 0 && value < columns; })) {
    return false;
  }

  // The same index cannot appear twice
  std::unordered_set<int32_t> indicesSet(permutation.begin(), permutation.end());
  if (indicesSet.size() != permutation.size()) {
    return false;
  }

  // Re-order matrix in-place
  std::vector<bool> isCorrectlyOrdered(matrix.size(), false);
  // All elements left of index i (with i included) have been correctly ordered
  for (int32_t i = 0; i < matrix.size(); ++i) {
    if (isCorrectlyOrdered[i]) {
      continue;
    }
    isCorrectlyOrdered[i] = true;
    int32_t k = i;
    int32_t j = permutation[i];
    while (i != j) {
      std::swap(matrix[k], matrix[j]);
      isCorrectlyOrdered[j] = true;
      k = j;
      j = permutation[j];
    }
  }


  return true;
}