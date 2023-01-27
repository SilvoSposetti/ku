#include "SudokuRow.h"

ConstraintType SudokuRow::getType() const {
  return ConstraintType::SUDOKU_ROW;
}

std::string SudokuRow::getName() const {
  return "Sudoku-Row";
}

std::string SudokuRow::getDescription() const {
  return "9x1 rows contain all the digits from 1 to 9 exactly once.";
}

std::string SudokuRow::getSvgGroup() const {
  std::string horizontalLines;
  const double cellSize = 1.0 / static_cast<double>(MAX_DIGIT);
  for (int i = 0; i < MAX_DIGIT; i++) {
    const double yPosition = cellSize * i;
    horizontalLines += SvgUtilities::line(0, yPosition, 1, yPosition);
  }
  return SvgUtilities::createGroup(getName(), horizontalLines, SvgUtilities::getNoFillStroke(thinLine));
}

bool SudokuRow::satisfy(const std::vector<std::vector<Sudo>>& board) const {
  // The board satisfies the constraint if all rows do not contain duplicate digits
  for (const auto& rowIndex : INDICES) {
    for (const auto& digit : SUDO_DIGITS) {
      int32_t count = 0;
      for (const auto& columnIndex : INDICES) {
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

int32_t SudokuRow::getDlxConstraintColumnsAmount() const {
  return MAX_DIGIT * MAX_DIGIT; // 9(rows) * 9(possible digits in each row)
}

bool SudokuRow::getDlxConstraint(Sudo digit, int32_t i, int32_t j, const int32_t columnId) const {
  // columnId encodes the (row, possible digit) pair
  const std::pair<int32_t, int32_t> unpacked = unpackId(columnId, MAX_DIGIT, MAX_DIGIT);
  const int32_t row = unpacked.first;
  const Sudo possibleDigit = static_cast<Sudo>(unpacked.second + 1);

  return row == i && possibleDigit == digit;
}
