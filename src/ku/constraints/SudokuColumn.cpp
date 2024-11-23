#include "SudokuColumn.hpp"

#include "../drawing/SvgLine.hpp"

SudokuColumn::SudokuColumn()
    : AbstractConstraint(ConstraintType::SUDOKU_COLUMN,
                         "Sudoku-Column",
                         "1x9 columns contain all the digits from 1 to 9 exactly once.") {}

std::unique_ptr<SvgGroup> SudokuColumn::getSvgGroup(const DrawingOptions& options) const {
  auto group = std::make_unique<SvgGroup>(getName(), std::nullopt, "black", options.thinLine);
  for (int32_t i = 0; i < Sudo::MAX_DIGIT; i++) {
    const double x = options.cellSize * i;
    group->add(std::make_unique<SvgLine>(x, 0.0, x, options.size));
  }
  return group;
}

bool SudokuColumn::satisfy(const std::vector<std::vector<Sudo::Digit>>& board) const {
  // The board satisfies the constraint if all columns do not contain duplicate digits
  for (const auto& columnIndex : Sudo::INDICES) {
    for (const auto& digit : Sudo::SUDO_DIGITS) {
      int32_t count = 0;
      for (const auto& rowIndex : Sudo::INDICES) {
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

int32_t SudokuColumn::definePrimaryItemsAmount() const {
  return Sudo::MAX_DIGIT * Sudo::MAX_DIGIT; // 9(columns) * 9(possible digits in each column)
}

std::vector<std::vector<int32_t>> SudokuColumn::definePrimaryItems() const {
  std::vector<std::vector<int32_t>> primaryItems(optionsOrdered().size());
  int32_t counter = 0;
  for (const auto& [rowIndex, columnIndex, digit] : optionsOrdered()) {
    primaryItems[counter] = {(columnIndex * Sudo::MAX_DIGIT + (static_cast<int32_t>(digit) - 1)) % Sudo::TOTAL_DIGITS};
    counter++;
  }
  return primaryItems;
}

int32_t SudokuColumn::defineSecondaryItemsAmount() const {
  return 0;
}

std::vector<std::vector<int32_t>> SudokuColumn::defineSecondaryItems() const {
  return {};
}