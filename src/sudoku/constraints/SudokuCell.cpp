#include "SudokuCell.h"

ConstraintType SudokuCell::getType() const {
  return ConstraintType::SUDOKU_CELL;
}

std::string SudokuCell::getName() const {
  return "Sudoku-Cell";
}

std::string SudokuCell::getDescription() const {
  return "Each cell can only contain one integer";
}

std::string SudokuCell::getSvgGroup() const {
  return "";
}

bool SudokuCell::validatePlacement(const Sudo digit,
                                   const int8_t rowIndex,
                                   const int8_t columnIndex,
                                   const std::vector<std::vector<Sudo>> board) const {
  // This is already enforced by the data structure used
  return true;
}

bool SudokuCell::satisfy(std::vector<std::vector<Sudo>> board) const {
  return true;
}

int32_t SudokuCell::getDLXConstraintColumnsAmount() const {
  return 9 * 9; // 9 * 9 = 81 single places in which a digit could go
}

bool SudokuCell::getDLXConstraint(const int8_t boardRow,
                                  const int8_t boardColumn,
                                  const int32_t columnId,
                                  const Sudo possibleDigit,
                                  const Sudo actualDigit) const {

  const int32_t matrixRow = columnId / (MAX_INDEX + 1);
  const int32_t matrixColumn = columnId % (MAX_INDEX + 1);

  if (actualDigit == Sudo::NONE) {
    return matrixRow == boardRow && matrixColumn == boardColumn;
  }
  return actualDigit == possibleDigit && matrixRow == boardRow && matrixColumn == boardColumn;
}
