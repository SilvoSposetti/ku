#include "SparseCoordinateMatrix.h"

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