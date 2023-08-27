#include "SparseCoordinateMatrix.h"

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <unordered_set>

SparseCoordinateMatrix::SparseCoordinateMatrix(const std::vector<std::vector<bool>>& classicMatrix,
                                               const std::unordered_set<int32_t>& secondaryColumnIndices,
                                               bool placeSecondaryColumnsAtEnd,
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

  if (!placeSecondaryColumnsAtEnd || secondaryColumnIndices.empty()) {
    // Set all elements
    for (std::size_t rowIndex = 0; rowIndex < rowsAmount; rowIndex++) {
      for (std::size_t columnIndex = 0; columnIndex < columnsAmount; columnIndex++) {
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
  } else {
    // Set all elements, but skip the secondary columns
    std::size_t currentColumn = 0;
    for (std::size_t columnIndex = 0; columnIndex < columnsAmount; columnIndex++) {
      if (!(secondaryColumnIndices.count(columnIndex) >= 1)) {
        for (std::size_t rowIndex = 0; rowIndex < rowsAmount; rowIndex++) {
          // Store the values in column-major order
          if (classicMatrix[rowIndex][columnIndex]) {
            matrix[currentColumn][rowIndex] = true;
            rowsElements[rowIndex].insert(currentColumn);
          }
        }
        currentColumn++;
      }
    }
    const std::size_t firstSecondaryColumnIndex = currentColumn;

    // Then, set all elements of the secondary columns
    std::vector<int32_t> sortedSecondaryColumns;
    for (const auto& element : secondaryColumnIndices) {
      sortedSecondaryColumns.emplace_back(element);
    }
    std::sort(sortedSecondaryColumns.begin(), sortedSecondaryColumns.end());
    for (const auto& secondaryColumnIndex : sortedSecondaryColumns) {
      for (std::size_t rowIndex = 0; rowIndex < rowsAmount; rowIndex++) {
        if (classicMatrix[rowIndex][secondaryColumnIndex]) {
          matrix[currentColumn][rowIndex] = true;
          rowsElements[rowIndex].insert(currentColumn);
        }
      }
      currentColumn++;
    }

    // Set all the secondary columns
    for (std::size_t secondaryColumnIndex = firstSecondaryColumnIndex; secondaryColumnIndex < columnsAmount;
         secondaryColumnIndex++) {
      areColumnsPrimary[secondaryColumnIndex] = false;
    }
  }

  // Store the given data for each row
  if (rowsDataVector.size() == rowsAmount) {
    rowsData = rowsDataVector;
  }
}

void SparseCoordinateMatrix::preprocess() {
  // TODO add Algorithm X preprocessing
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

  // Matrix is not solvable if the secondary columns are not at the end
  auto firstSecondaryElementIterator = std::find(areColumnsPrimary.begin(), areColumnsPrimary.end(), false);
  if (firstSecondaryElementIterator != areColumnsPrimary.end()) {
    if (std::any_of(firstSecondaryElementIterator, areColumnsPrimary.end(), [](bool value) { return value; })) {
      std::cout << "Matrix not solvable: the secondary columns are not all at the end" << std::endl;
      return false;
    };
  }

  return true;
}

int32_t SparseCoordinateMatrix::getRowData(int32_t rowIndex) const {
  if (isValidRowIndex(rowIndex)) {
    return rowsData[rowIndex];
  }
  return invalidRowValue;
};

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