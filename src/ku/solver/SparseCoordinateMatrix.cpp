#include "SparseCoordinateMatrix.h"

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <unordered_set>

SparseCoordinateMatrix::SparseCoordinateMatrix(const std::vector<std::vector<bool>>& classicMatrix,
                                               const std::unordered_set<int32_t>& secondaryColumnIndices,
                                               const std::vector<int32_t>& rowsDataVector)
    : rowsAmount(classicMatrix.size())
    , columnsAmount(!classicMatrix.empty() ? classicMatrix[0].size() : 0)
    , matrix(columnsAmount, std::vector<bool>(rowsAmount, false)) // Initialize all rows data to -1
    , rowsElements(rowsAmount, std::set<int32_t>())
    , rowsData(rowsAmount, invalidRowValue)
    , areColumnsPrimary(columnsAmount, true) {

  const bool isMatrixMalformed = std::any_of(classicMatrix.begin(),
                                             classicMatrix.end(),
                                             [&](const std::vector<bool>& row) { return row.size() != columnsAmount; });
  if (isMatrixMalformed) {
    rowsAmount = 0;
    columnsAmount = 0;
    matrix.clear();
    rowsElements.clear();
    rowsData.clear();
    areColumnsPrimary.clear();
    return;
  }

  for (std::size_t rowIndex = 0; rowIndex < classicMatrix.size(); rowIndex++) {
    for (std::size_t columnIndex = 0; columnIndex < classicMatrix[rowIndex].size(); columnIndex++) {
      // Store the values in column-major order
      if (classicMatrix[rowIndex][columnIndex]) {
        matrix[columnIndex][rowIndex] = true;
        rowsElements[rowIndex].insert(columnIndex);
      }
    }
  }

  // Set all the secondary columns
  for (const int32_t secondaryColumnIndex : secondaryColumnIndices) {
    if (isValidColumnIndex(secondaryColumnIndex)) {
      areColumnsPrimary[secondaryColumnIndex] = false;
    }
  }

  // Store the given data for each row
  if (rowsDataVector.size() == rowsAmount) {
    rowsData = rowsDataVector;
  }
}

void preprocess(std::shared_ptr<RandomGenerator> randomGenerator) {
  // TODO do some Algorithm X preprocessing
  std::cout << "" << std::endl;
}

int32_t SparseCoordinateMatrix::getColumnsAmount() const {
  return columnsAmount;
}

int32_t SparseCoordinateMatrix::getRowsAmount() const {
  return rowsAmount;
}

int32_t SparseCoordinateMatrix::getValidElementsAmount() const {
  int32_t counter = 0;
  for (const auto& rowElements : rowsElements) {
    counter += rowElements.size();
  }
  return counter;
}

bool SparseCoordinateMatrix::isColumnPrimary(int32_t columnIndex) const {
  if (isValidColumnIndex(columnIndex)) {
    return areColumnsPrimary[columnIndex];
  }
  return false;
}

bool SparseCoordinateMatrix::isCellSet(int32_t rowIndex, int32_t columnIndex) const {
  // Return the empty cell when the indices are not valid
  if (!isValidRowIndex(rowIndex) || !isValidColumnIndex(columnIndex)) {
    return false;
  }
  // Retrieve value
  return matrix[columnIndex][rowIndex];
}

bool SparseCoordinateMatrix::isSolvableByAlgorithmX() const {
  if (columnsAmount == 0 || rowsAmount == 0 || matrix.empty() || rowsElements.empty()) {
    return true;
  }

  // Matrix is not solvable if all columns are secondary
  if (std::all_of(areColumnsPrimary.begin(), areColumnsPrimary.end(), [](bool value) { return !value; })) {
    std::cout << "Matrix not solvable: all columns are secondary" << std::endl;
    return false;
  }

  // Matrix is not solvable if any of the primary columns is empty
  for (std::size_t columnId = 0; columnId < columnsAmount; columnId++) {
    if (areColumnsPrimary[columnId]) {
      if (std::all_of(matrix[columnId].begin(), matrix[columnId].end(), [](bool value) { return !value; })) {
        std::cout << "Matrix not solvable: a primary column is empty." << std::endl;
        return false;
      }
    }
  }

  return true;
}

int32_t SparseCoordinateMatrix::getRowData(int32_t rowIndex) const {
  if (isValidRowIndex(rowIndex)) {
    return rowsData[rowIndex];
  }
  return invalidRowValue;
};

bool SparseCoordinateMatrix::reorderColumns(const std::vector<int32_t>& permutation) {
  // Permutation's size must be the same as the amount of columns in the matrix
  if (permutation.size() != columnsAmount) {
    return false;
  }

  // All indices in the permutation must be valid indices
  if (std::any_of(permutation.begin(), permutation.end(), [&](int32_t columnIndex) {
        return !isValidColumnIndex(columnIndex);
      })) {
    return false;
  }

  // The same index cannot appear twice
  std::unordered_set<int32_t> indicesSet(permutation.begin(), permutation.end());
  if (indicesSet.size() != permutation.size()) {
    return false;
  }

  // Re-order matrix in-place
  std::vector<bool> isCorrectlyOrdered(columnsAmount, false);
  // All elements left of index i (with i included) have been correctly ordered
  for (std::size_t i = 0; i < columnsAmount; ++i) {
    if (isCorrectlyOrdered[i]) {
      continue;
    }
    isCorrectlyOrdered[i] = true;
    std::size_t k = i;
    std::size_t j = permutation[i];
    while (i != j) {
      std::swap(matrix[k], matrix[j]);
      areColumnsPrimary.swap(areColumnsPrimary[k], areColumnsPrimary[j]);
      isCorrectlyOrdered[j] = true;
      k = j;
      j = permutation[j];
    }
  }

  // Re-order the rowsElements by substituting them with a new one created using the permutation
  std::unordered_map<int32_t, int32_t> map;
  for (std::size_t i = 0; i < permutation.size(); i++) {
    map.insert({permutation[i], i});
  }

  std::vector<std::set<int32_t>> newRowsElements;
  for (const auto& rowElements : rowsElements) {
    std::set<int32_t> newRow;
    for (const auto& element : rowElements) {
      newRow.insert(map[element]);
    }
    newRowsElements.emplace_back(newRow);
  }
  rowsElements = newRowsElements;

  return true;
}

int32_t SparseCoordinateMatrix::getColumnValidCellsAmount(int32_t columnIndex) const {
  if (isValidColumnIndex(columnIndex)) {
    return std::count(matrix[columnIndex].begin(), matrix[columnIndex].end(), true);
  }
  return -1;
}

bool SparseCoordinateMatrix::isValidColumnIndex(int32_t columnIndex) const {
  return 0 <= static_cast<std::size_t>(columnIndex) && static_cast<std::size_t>(columnIndex) < columnsAmount;
}

bool SparseCoordinateMatrix::isValidRowIndex(int32_t rowIndex) const {
  return 0 <= static_cast<std::size_t>(rowIndex) && static_cast<std::size_t>(rowIndex) < rowsAmount;
}