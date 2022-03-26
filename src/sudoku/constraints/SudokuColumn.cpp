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

bool SudokuColumn::validatePlacement(const Sudo digit,
                                     const int32_t rowIndex,
                                     const int32_t columnIndex,
                                     const std::vector<std::vector<Sudo>> board) const {
  // If the digit is already present in the given row, then the placement is not valid
  for (const auto& index : INDICES) {
    if (board[index][columnIndex] == digit) {
      return false;
    }
  }
  return true;
}

bool SudokuColumn::satisfy(std::vector<std::vector<Sudo>> board) const {
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

bool SudokuColumn::getDLXConstraint(const int32_t boardRow,
                                    const int32_t boardColumn,
                                    const int32_t columnId,
                                    const Sudo possibleDigit,
                                    const Sudo actualDigit) const {
  const int32_t matrixRow = columnId / MAX_DIGIT;
  const int32_t matrixColumn = columnId % MAX_DIGIT;

  if (actualDigit == Sudo::NONE) {
    return matrixRow == boardColumn && matrixColumn == static_cast<int32_t>(possibleDigit) - 1;
  }
  return actualDigit == possibleDigit && matrixRow == boardColumn &&
         matrixColumn == static_cast<int32_t>(possibleDigit) - 1;
}
