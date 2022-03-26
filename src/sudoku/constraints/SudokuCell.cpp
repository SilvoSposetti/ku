#include "SudokuCell.h"

ConstraintType SudokuCell::getType() const {
  return ConstraintType::SUDOKU_CELL;
}

std::string SudokuCell::getName() const {
  return "Sudoku-Cell";
}

std::string SudokuCell::getDescription() const {
  return "Each cell can only contain one integer between 1 and 9.";
}

std::string SudokuCell::getSvgGroup() const {
  const std::string rect = SvgUtilities::rect(0, 0, 1, 1);
  return SvgUtilities::createGroup(getName(), rect, SvgUtilities::getNoFillStroke(thickLine));
}

bool SudokuCell::validatePlacement(Sudo digit,
                                   int32_t rowIndex,
                                   int32_t columnIndex,
                                   const std::vector<std::vector<Sudo>>& board) const {
  // This is already enforced by the data structure used
  return true;
}

bool SudokuCell::satisfy(const std::vector<std::vector<Sudo>>& board) const {
  return true;
}

int32_t SudokuCell::getDLXConstraintColumnsAmount() const {
  return 9 * 9; // 9 * 9 = 81 single places in which a digit could go
}

bool SudokuCell::getDLXConstraint(Sudo digit, int32_t i, int32_t j, const int32_t columnId) const {

  // columnId encodes the location of a digit on the board
  const int32_t matrixRow = columnId / MAX_DIGIT;
  const int32_t matrixColumn = columnId % MAX_DIGIT;

  return matrixRow == i && matrixColumn == j;
}
