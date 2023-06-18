#include "SparseCoordinateMatrix.h"

#include <algorithm>
#include <iostream>
#include <unordered_set>

SparseCoordinateMatrix::SparseCoordinateMatrix(int32_t totalRows, int32_t totalColumns)
    : columns(totalColumns)
    , columnsAmount(totalColumns)
    , rowsAmount(totalRows) {}

SparseCoordinateMatrix::SparseCoordinateMatrix(const std::vector<std::vector<int32_t>>& matrix) {
  rowsAmount = matrix.size();
  if (rowsAmount == 0) {
    reset();
    return;
  }
  columnsAmount = matrix[0].size();
  columns = std::vector<SparseCoordinateColumn>(columnsAmount);

  for (size_t i = 0; i < matrix.size(); i++) {
    if (matrix[i].size() != matrix[0].size()) {
      reset();
      return;
    }
    for (size_t j = 0; j < matrix[i].size(); j++) {
      setCell(i, j, matrix[i][j]);
    }
  }

  if (getValidElementsAmount() == 0) {
    reset();
  }
}

int32_t SparseCoordinateMatrix::getColumnsAmount() const {
  return columnsAmount;
}

int32_t SparseCoordinateMatrix::getRowsAmount() const {
  return rowsAmount;
}

int32_t SparseCoordinateMatrix::getValidElementsAmount() const {
  int32_t counter = 0;
  for (const auto& column : columns) {
    counter += column.elements.size();
  }
  return counter;
}

void SparseCoordinateMatrix::setColumnSecondary(int32_t columnIndex) {
  if (0 <= columnIndex && columnIndex < columnsAmount) {
    columns[columnIndex].isPrimary = false;
  }
}

bool SparseCoordinateMatrix::isColumnPrimary(int32_t columnIndex) const {
  if (0 <= columnIndex && columnIndex < columnsAmount) {
    return columns[columnIndex].isPrimary;
  }
  return false;
}

bool SparseCoordinateMatrix::setCell(int32_t rowIndex, int32_t columnIndex, int32_t data) {
  // Do not insert if the cell is outside of the matrix range
  const bool isWithinRange =
      (0 <= rowIndex && rowIndex < rowsAmount) && (0 <= columnIndex && columnIndex < columnsAmount);
  if (!isWithinRange) {
    return false;
  }

  if (data >= 0) {
    // Add the element to the set
    columns[columnIndex].elements.insert(std::make_pair(rowIndex, data));
  } else {
    columns[columnIndex].elements.erase(rowIndex);
  }

  return true;
}

int32_t SparseCoordinateMatrix::getCell(int32_t rowIndex, int32_t columnIndex) const {
  // Return the empty cell when the indices are not valid
  if (0 > rowIndex || rowIndex > rowsAmount - 1 || 0 > columnIndex || columnIndex > columnsAmount - 1) {
    return -1;
  }
  // Retrieve value
  const auto& elements = columns[columnIndex].elements;
  const auto& foundElement = elements.find(rowIndex);
  if (foundElement != elements.end()) {
    return foundElement->second;
  }

  return -1;
}

bool SparseCoordinateMatrix::isSolvableByAlgorithmX() const {
  if (columnsAmount == 0 || rowsAmount == 0 || columns.size() == 0) {
    return true;
  }

  // Matrix is not solvable if all columns are secondary
  bool allSecondary = true;
  for (const auto& column : columns) {
    if (column.isPrimary) {
      allSecondary = false;
      break;
    }
  }
  if (allSecondary) {
    std::cout << "Matrix not solvable: all columns are secondary" << std::endl;
    return false;
  }

  // Matrix is not solvable if any of the primary columns is empty
  int32_t columnIndex = 0;
  std::vector<int32_t> emptyPrimaryColumnsIndices;
  for (const auto& column : columns) {
    if (column.isPrimary && column.elements.size() == 0) {
      emptyPrimaryColumnsIndices.emplace_back(columnIndex);
    }
    columnIndex++;
  }
  if (!emptyPrimaryColumnsIndices.empty()) {
    std::string s = "[";
    const int32_t size = emptyPrimaryColumnsIndices.size();
    for (int32_t i = 0; i < size; i++) {
      s += std::to_string(emptyPrimaryColumnsIndices[i]);
      if (i != size - 1)
        s += ", ";
    }
    s += "]";
    std::cout << "Matrix not solvable: the following primary columns indices are empty " + s << std::endl;
  }

  return emptyPrimaryColumnsIndices.empty();
}

bool SparseCoordinateMatrix::reorderColumns(const std::vector<int32_t>& permutation) {
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

void SparseCoordinateMatrix::sortPrimaryAndSecondaryColumns() {
  // Gather column information, in the form of lists of {columnIndex, columnSize} for primary and secondary items
  std::vector<std::pair<int32_t, int32_t>> primaryColumns;
  std::vector<std::pair<int32_t, int32_t>> secondaryColumns;
  for (int32_t i = 0; i < columnsAmount; i++) {
    if (columns[i].isPrimary) {
      primaryColumns.emplace_back(std::make_pair(i, columns[i].elements.size()));
    } else {
      secondaryColumns.emplace_back(std::make_pair(i, columns[i].elements.size()));
    }
  }

  // Sort both lists by the sizes
  const auto sortByColumnSize = [&](const std::pair<int32_t, int32_t>& left,
                                    const std::pair<int32_t, int32_t>& right) -> bool {
    return left.second < right.second;
  };
  std::sort(primaryColumns.begin(), primaryColumns.end(), sortByColumnSize);
  std::sort(secondaryColumns.begin(), secondaryColumns.end(), sortByColumnSize);

  // Create permutation vector
  std::vector<int32_t> permutation;
  for (const auto& p : primaryColumns) {
    permutation.emplace_back(p.first);
  }
  for (const auto& s : secondaryColumns) {
    permutation.emplace_back(s.first);
  }

  reorderColumns(permutation);
}

void SparseCoordinateMatrix::reset() {
  columnsAmount = 0;
  rowsAmount = 0;
  columns.clear();
}