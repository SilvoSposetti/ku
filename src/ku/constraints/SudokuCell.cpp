#include "SudokuCell.h"

ConstraintType SudokuCell::getType() const {
  return ConstraintType::SUDOKU_CELL;
}

std::string SudokuCell::getDescription() const {
  return "Each cell can only contain one integer between 1 and 9.";
}

std::string SudokuCell::getSvgGroup() const {
  // Black border
  std::string border = SvgUtilities::rect(0, 0, 1, 1, SvgUtilities::getNoFillStroke(thickLine));

  // dashed-lines grid
  std::string grid;
  const double cellSize = 1.0 / Sudo::MAX_DIGIT;
  for (int i = 1; i <= Sudo::MAX_INDEX; i++) {
    const double position = cellSize * i;
    grid += SvgUtilities::line(position, 0, position, 1);
    grid += SvgUtilities::line(0, position, 1, position);
  }
  grid = SvgUtilities::createGroup(getName() + "-Grid", grid, SvgUtilities::getNoFillDashedStroke(thinnestLine, 3, 6));
  return SvgUtilities::createGroup(getName(), border + grid);
}

bool SudokuCell::satisfy(const std::vector<std::vector<Sudo::Digit>>& board) const {
  return true;
}

int32_t SudokuCell::getItemsAmount() const {
  return Sudo::TOTAL_DIGITS; // 9 * 9 = 81 single places in which a digit could go
}

bool SudokuCell::computeConstraint(Sudo::Digit digit, int32_t i, int32_t j, const int32_t itemId) const {

  // columnId encodes the location (row, column) pair of a digit on the board
  const std::pair<int32_t, int32_t> unpacked = IdPacking::unpackId(itemId, Sudo::MAX_DIGIT, Sudo::MAX_DIGIT);
  const int32_t row = unpacked.first;
  const int32_t column = unpacked.second;

  return row == i && column == j;
}
