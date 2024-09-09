#include "SudokuCell.h"

#include "../drawing/SvgRect.h"

SudokuCell::SudokuCell()
    : AbstractConstraint(
          ConstraintType::SUDOKU_CELL, "Sudoku-Cell", "Each cell can only contain a single integer between 1 and 9.") {}

std::unique_ptr<SvgGroup> SudokuCell::getSvgGroup(const DrawingOptions& options) const {
  auto group = std::make_unique<SvgGroup>(getName(), "transparent", "black", options.thickLine);
  group->add(std::make_unique<SvgRect>(0.0, 0.0, options.size, options.size));
  return group;
}

bool SudokuCell::satisfy(const std::vector<std::vector<Sudo::Digit>>& board) const {
  return true;
}

int32_t SudokuCell::definePrimaryItemsAmount() const {
  return Sudo::TOTAL_DIGITS; // 9 * 9 = 81 single places in which a digit could go
}

std::vector<std::vector<int32_t>> SudokuCell::definePrimaryItems() const {
  std::vector<std::vector<int32_t>> primaryItems(optionsOrdered().size());

  int32_t counter = 0;
  for (const auto& [rowIndex, columnIndex, digit] : optionsOrdered()) {
    primaryItems[counter] = {(rowIndex * Sudo::MAX_DIGIT + columnIndex) % Sudo::TOTAL_DIGITS};
    counter++;
  }
  return primaryItems;
}

int32_t SudokuCell::defineSecondaryItemsAmount() const {
  return 0;
}

std::vector<std::vector<int32_t>> SudokuCell::defineSecondaryItems() const {
  return {};
}