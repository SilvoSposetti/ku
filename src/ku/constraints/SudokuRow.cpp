#include "SudokuRow.h"

#include "../drawing/SvgLine.h"

SudokuRow::SudokuRow()
    : AbstractConstraint(
          ConstraintType::SUDOKU_ROW, "Sudoku-Row", "9x1 rows contain all the digits from 1 to 9 exactly once.") {}

std::unique_ptr<SvgGroup> SudokuRow::getSvgGroup(const DrawingOptions& options) const {
  auto group = std::make_unique<SvgGroup>(getName(), std::nullopt, "black", options.thinLine);
  for (int32_t i = 0; i < Sudo::MAX_DIGIT; i++) {
    const double y = options.cellSize * i;
    group->add(std::make_unique<SvgLine>(0, y, options.size, y, std::nullopt, std::nullopt));
  }
  return group;
}

bool SudokuRow::satisfy(const std::vector<std::vector<Sudo::Digit>>& board) const {
  // The board satisfies the constraint if all rows do not contain duplicate digits
  for (const auto& rowIndex : Sudo::INDICES) {
    for (const auto& digit : Sudo::SUDO_DIGITS) {
      int32_t count = 0;
      for (const auto& columnIndex : Sudo::INDICES) {
        if (board[rowIndex][columnIndex] == digit) {
          count++;
        }
      }
      if (count != 1) {
        return false;
      }
    }
  }
  return true;
}

int32_t SudokuRow::definePrimaryItemsAmount() const {
  return Sudo::MAX_DIGIT * Sudo::MAX_DIGIT; // 9(rows) * 9(possible digits in each row)
}

std::vector<std::vector<int32_t>> SudokuRow::definePrimaryItems() const {
  std::vector<std::vector<int32_t>> primaryItems(optionsOrdered().size());
  int32_t counter = 0;
  for (const auto& [rowIndex, columnIndex, digit] : optionsOrdered()) {
    primaryItems[counter] = {(rowIndex * Sudo::MAX_DIGIT + (static_cast<int32_t>(digit) - 1)) % Sudo::TOTAL_DIGITS};
    counter++;
  }
  return primaryItems;
}

int32_t SudokuRow::defineSecondaryItemsAmount() const {
  return 0;
}

std::vector<std::vector<int32_t>> SudokuRow::defineSecondaryItems() const {
  return {};
}