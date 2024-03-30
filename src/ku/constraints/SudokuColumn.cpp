#include "SudokuColumn.h"

ConstraintType SudokuColumn::getType() const {
  return ConstraintType::SUDOKU_COLUMN;
}

std::string SudokuColumn::getDescription() const {
  return "1x9 columns contain all the digits from 1 to 9 exactly once.";
}

std::string SudokuColumn::getSvgGroup() const {
  std::string verticalLines;
  const double cellSize = 1.0 / static_cast<double>(Sudo::MAX_DIGIT);
  for (int i = 0; i < Sudo::MAX_DIGIT; i++) {
    const double xPosition = cellSize * i;
    verticalLines += SvgUtilities::line(xPosition, 0, xPosition, 1);
  }
  return SvgUtilities::createGroup(getName(), verticalLines, SvgUtilities::getNoFillStroke(thinLine));
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

int32_t SudokuColumn::getItemsAmount() const {
  return Sudo::MAX_DIGIT * Sudo::MAX_DIGIT; // 9(columns) * 9(possible digits in each column)
}

bool SudokuColumn::computeConstraint(Sudo::Digit digit, int32_t i, int32_t j, const int32_t itemId) const {

  // columnId encodes the (column, possible digit) pair
  const std::pair<int32_t, int32_t> unpacked = IdPacking::unpackId(itemId, Sudo::MAX_DIGIT, Sudo::MAX_DIGIT);
  const int32_t column = unpacked.first;
  const Sudo::Digit possibleDigit = static_cast<Sudo::Digit>(unpacked.second + 1);

  return column == j && possibleDigit == digit;
}
