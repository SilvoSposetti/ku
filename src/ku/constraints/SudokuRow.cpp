#include "SudokuRow.h"

SudokuRow::SudokuRow()
    : AbstractConstraint(
          ConstraintType::SUDOKU_ROW, "Sudoku-Row", "9x1 rows contain all the digits from 1 to 9 exactly once.") {}

std::string SudokuRow::getSvgGroup() const {
  std::string horizontalLines;
  const double cellSize = 1.0 / static_cast<double>(Sudo::MAX_DIGIT);
  for (int i = 0; i < Sudo::MAX_DIGIT; i++) {
    const double yPosition = cellSize * i;
    horizontalLines += SvgUtilities::line(0, yPosition, 1, yPosition);
  }
  return SvgUtilities::createGroup(getName(), horizontalLines, SvgUtilities::getNoFillStroke(thinLine));
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

int32_t SudokuRow::getPrimaryItemsAmount() const {
  return Sudo::MAX_DIGIT * Sudo::MAX_DIGIT; // 9(rows) * 9(possible digits in each row)
}

std::vector<std::vector<int32_t>> SudokuRow::getPrimaryItems() const {
  std::vector<std::vector<int32_t>> primaryItems(optionsOrdered().size());
  int32_t counter = 0;
  for (const auto& [rowIndex, columnIndex, digit] : optionsOrdered()) {
    primaryItems[counter] = {(rowIndex * Sudo::MAX_DIGIT + (static_cast<int32_t>(digit) - 1)) % Sudo::TOTAL_DIGITS};
    counter++;
  }
  return primaryItems;
}

int32_t SudokuRow::getSecondaryItemsAmount() const {
  return 0;
}

std::vector<std::vector<int32_t>> SudokuRow::getSecondaryItems() const {
  return{};
}