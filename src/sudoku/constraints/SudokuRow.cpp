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
  return SvgUtilities::createGroup(getName(), horizontalLines, noFillThinStroke);
}

bool SudokuRow::validatePlacement(const Sudo digit,
                                  const int8_t rowIndex,
                                  const int8_t columnIndex,
                                  const std::vector<std::vector<Sudo>> board) const {
  // If the digit is already present in the column, then the placement is not valid
  for (const auto& index : INDICES) {
    if (board[rowIndex][index] == digit) {
      return false;
    }
  }
  return true;
}

bool SudokuRow::satisfy(std::vector<std::vector<Sudo>> board) const {
  // The board satisfies the constraint if all rows do not contain duplicate digits
  for (const auto& rowIndex : INDICES) {
    for (const auto& digit : SUDO_DIGITS) {
      int8_t count = 0;
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

int32_t SudokuRow::getDLXConstraintColumnsAmount() const {
  return 9 * 9; // 9(rows) * 9(possible digits in each row)
}

bool SudokuRow::getDLXConstraint(const int8_t boardRow,
                                 const int8_t boardColumn,
                                 const int32_t columnId,
                                 const Sudo possibleDigit,
                                 const Sudo actualDigit) const {

  const int32_t matrixRow = columnId / (MAX_INDEX + 1);
  const int32_t matrixColumn = columnId % (MAX_INDEX + 1);

  if (actualDigit == Sudo::NONE) {
    return matrixRow == boardRow && matrixColumn == static_cast<int32_t>(possibleDigit) - 1;
  }
  return actualDigit == possibleDigit && matrixRow == boardRow &&
         matrixColumn == static_cast<int32_t>(possibleDigit) - 1;
}