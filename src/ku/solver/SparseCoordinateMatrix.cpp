#include "SparseCoordinateMatrix.h"

#include <unordered_set>

SparseCooordinateMatrix::SparseCooordinateMatrix(int32_t totalRows, int32_t totalColumns)
    : columns(totalColumns)
    , columnsAmount(totalColumns)
    , rowsAmount(totalRows) {}

int32_t SparseCooordinateMatrix::getColumnsAmount() const {
  return columnsAmount;
}

int32_t SparseCooordinateMatrix::getRowsAmount() const {
  return rowsAmount;
}

void SparseCooordinateMatrix::setColumnSecondary(int32_t columnIndex) {
  if (0 <= columnIndex && columnIndex < columns.size()) {
    columns[columnIndex].isColumnPrimary = false;
  }
}

bool SparseCooordinateMatrix::isColumnPrimary(int32_t columnIndex) const {
  if (0 <= columnIndex && columnIndex < columns.size()) {
    return columns[columnIndex].isColumnPrimary;
  }
  return false;
}

bool SparseCooordinateMatrix::setData(int32_t rowIndex, int32_t columnIndex, int32_t data) {
  // Do not insert if the cell is outside of the matrix range
  if (0 > rowIndex || rowIndex > rowsAmount - 1 || 0 > columnIndex || columnIndex > columnsAmount - 1) {
    return false;
  }

  std::vector<SparseCooordinateCell>& column = columns[columnIndex].elements;
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
  if (0 > rowIndex || rowIndex > rowsAmount - 1 || 0 > columnIndex || columnIndex > columnsAmount - 1) {
    return -1;
  }
  // Retrieve value
  const auto& columnElements = columns[columnIndex].elements;
  for (const auto& cell : columnElements) {
    if (cell.matrixRow == rowIndex) {
      return cell.value;
    }
  }
  return -1;
}

bool SparseCooordinateMatrix::reorderColumns(const std::vector<int32_t>& permutation) {
  // Permutation's size must be the same as the amount of columns in the matrix
  if (static_cast<int32_t>(permutation.size()) != columnsAmount) {
    return false;
  }

  // All indices in the permutation must be valid indices
  if (!std::all_of(
          permutation.begin(), permutation.end(), [&](int32_t value) { return value >= 0 && value < columnsAmount; })) {
    return false;
  }

  // The same index cannot appear twice
  std::unordered_set<int32_t> indicesSet(permutation.begin(), permutation.end());
  if (indicesSet.size() != permutation.size()) {
    return false;
  }

  // Re-order matrix in-place
  std::vector<bool> isCorrectlyOrdered(columns.size(), false);
  // All elements left of index i (with i included) have been correctly ordered
  for (int32_t i = 0; i < columns.size(); ++i) {
    if (isCorrectlyOrdered[i]) {
      continue;
    }
    isCorrectlyOrdered[i] = true;
    int32_t k = i;
    int32_t j = permutation[i];
    while (i != j) {
      std::swap(columns[k], columns[j]);
      isCorrectlyOrdered[j] = true;
      k = j;
      j = permutation[j];
    }
  }

  return true;
}