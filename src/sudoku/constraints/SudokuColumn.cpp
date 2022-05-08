#include "SudokuColumn.h"

ConstraintType SudokuColumn::getType() const {
  return ConstraintType::SUDOKU_COLUMN;
}

std::string SudokuColumn::getName() const {
  return "Sudoku-Column";
}

std::string SudokuColumn::getDescription() const {
  return "1x9 columns contain all the digits from 1 to 9 exactly once.";
}

std::string SudokuColumn::getSvgGroup() const {
  std::string verticalLines;
  const double cellSize = 1.0 / static_cast<double>(MAX_DIGIT);
  for (int i = 0; i < MAX_DIGIT; i++) {
    const double xPosition = cellSize * i;
    verticalLines += SvgUtilities::line(xPosition, 0, xPosition, 1);
  }
  return SvgUtilities::createGroup(getName(), verticalLines, SvgUtilities::getNoFillStroke(thinLine));
}

bool SudokuColumn::satisfy(const std::vector<std::vector<Sudo>>& board) const {
  // The board satisfies the constraint if all columns do not contain duplicate digits
  for (const auto& columnIndex : INDICES) {
    for (const auto& digit : SUDO_DIGITS) {
      int32_t count = 0;
      for (const auto& rowIndex : INDICES) {
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

int32_t SudokuColumn::getDLXConstraintColumnsAmount() const {
  return 9 * 9; // 9(columns) * 9(possible digits in each column)
}

bool SudokuColumn::getDLXConstraint(Sudo digit, int32_t i, int32_t j, const int32_t columnId) const {

  // columnId encodes the (column, possible digit) pair
  const std::pair<int32_t, int32_t> unpacked = unpackId(columnId, MAX_DIGIT, MAX_DIGIT);
  const int32_t column = unpacked.first;
  const Sudo possibleDigit = static_cast<Sudo>(unpacked.second + 1);

  return column == j && possibleDigit == digit;
}
