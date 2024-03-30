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

int32_t SudokuRow::getItemsAmount() const {
  return Sudo::MAX_DIGIT * Sudo::MAX_DIGIT; // 9(rows) * 9(possible digits in each row)
}

bool SudokuRow::computeConstraint(Sudo::Digit digit, int32_t i, int32_t j, const int32_t itemId) const {
  // itemId encodes the (row, possible digit) pair
  const std::pair<int32_t, int32_t> unpacked = IdPacking::unpackId(itemId, Sudo::MAX_DIGIT, Sudo::MAX_DIGIT);
  const int32_t row = unpacked.first;
  const Sudo::Digit possibleDigit = static_cast<Sudo::Digit>(unpacked.second + 1);

  return row == i && possibleDigit == digit;
}
