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

bool SudokuCell::hasOptionalConstraints() const {
  return false;
}

std::string SudokuCell::getSvgGroup() const {
  const std::string rect = SvgUtilities::rect(0, 0, 1, 1);
  return SvgUtilities::createGroup(getName(), rect, SvgUtilities::getNoFillStroke(thickLine));
}

bool SudokuCell::satisfy(const std::vector<std::vector<Sudo>>& board) const {
  return true;
}

int32_t SudokuCell::getDlxConstraintColumnsAmount() const {
  return TOTAL_DIGITS; // 9 * 9 = 81 single places in which a digit could go
}

bool SudokuCell::getDlxConstraint(Sudo digit, int32_t i, int32_t j, const int32_t columnId) const {

  // columnId encodes the location (row, column) pair of a digit on the board
  const std::pair<int32_t, int32_t> unpacked = unpackId(columnId, MAX_DIGIT, MAX_DIGIT);
  const int32_t row = unpacked.first;
  const int32_t column = unpacked.second;

  return row == i && column == j;
}
