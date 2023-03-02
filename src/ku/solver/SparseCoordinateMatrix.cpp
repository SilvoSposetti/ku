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
  if (0 <= columnIndex && columnIndex < columnsAmount) {
    columns[columnIndex].isColumnPrimary = false;
  }
}

bool SparseCooordinateMatrix::isColumnPrimary(int32_t columnIndex) const {
  if (0 <= columnIndex && columnIndex < columnsAmount) {
    return columns[columnIndex].isColumnPrimary;
  }
  return false;
}

bool SparseCooordinateMatrix::setCell(int32_t rowIndex, int32_t columnIndex, bool data) {
  // Do not insert if the cell is outside of the matrix range
  const bool isWithinRange =
      (0 <= rowIndex && rowIndex < rowsAmount) && (0 <= columnIndex && columnIndex < columnsAmount);
  if (!isWithinRange) {
    return false;
  }

  if (data == true) {
    // Add the element to the set
    columns[columnIndex].elements.insert(rowIndex);
  } else {
    columns[columnIndex].elements.erase(rowIndex);
  }

  return true;
}

bool SparseCooordinateMatrix::getCell(int32_t rowIndex, int32_t columnIndex) const {
  // Return 0 when the indices are not valid
  if (0 > rowIndex || rowIndex > rowsAmount - 1 || 0 > columnIndex || columnIndex > columnsAmount - 1) {
    return false;
  }
  // Retrieve value
  const auto& elements = columns[columnIndex].elements;
  return elements.find(rowIndex) != elements.end();
}

bool SparseCooordinateMatrix::isSolvableByDlx() const {

  // Matrix is not solvable if all coloumns are secondary
  bool allSecondary = true;
  for (const auto& column : columns) {
    if (column.isColumnPrimary) {
      allSecondary = false;
    }
  }
  if (allSecondary) {
    std::cout << "Cannot solve matrix, all columns are secondary" << std::endl;
    return false;
  }

  // Matrix is not solvable if any of the primary columns is empty
  bool anyPrimaryColumnIsEmpty = false;
  int32_t columnIndex = 0;
  for (const auto& column : columns) {
    if (column.isColumnPrimary && column.elements.size() == 0) {
      std::cout << "Cannot solve matrix, one of the primary columns contains only unset cells! (the one at index "
                << columnIndex << ")" << std::endl;
      anyPrimaryColumnIsEmpty = true;
    }
  }
  return !anyPrimaryColumnIsEmpty;
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
  for (int32_t i = 0; i < columnsAmount; ++i) {
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