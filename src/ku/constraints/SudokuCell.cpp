#include "SudokuCell.h"

SudokuCell::SudokuCell()
    : AbstractConstraint(
          ConstraintType::SUDOKU_CELL, "Sudoku-Cell", "Each cell can only contain a single integer between 1 and 9.") {}

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

int32_t SudokuCell::getPrimaryItemsAmount() const {
  return Sudo::TOTAL_DIGITS; // 9 * 9 = 81 single places in which a digit could go
}

std::vector<std::vector<int32_t>> SudokuCell::getPrimaryItems() const {
  std::vector<std::vector<int32_t>> primaryItems(optionsOrdered().size());

  int32_t counter = 0;
  for (const auto& [i, j, digit] : optionsOrdered()) {
    primaryItems[counter] = {(i * Sudo::MAX_DIGIT + j) % Sudo::TOTAL_DIGITS};
    counter++;
  }
  return primaryItems;
}

int32_t SudokuCell::getSecondaryItemsAmount() const {
  return 0;
}

std::vector<std::vector<int32_t>> SudokuCell::getSecondaryItems() const {
  return {};
}