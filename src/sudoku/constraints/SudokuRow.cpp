#include "SudokuRow.h"

ConstraintType SudokuRow::getType() {
    return ConstraintType::SUDOKU_ROW;
}

std::string SudokuRow::getName() {
    return "Sudoku-Row";
}

std::string SudokuRow::getDescription() {
    return "9x1 rows contain all the digits from 1 to 9 exactly once.";
}

std::string SudokuRow::getSvgGroup() {
    return "";
}

bool SudokuRow::validatePlacement(const Sudo digit,
                                  const int8_t rowIndex,
                                  const int8_t columnIndex,
                                  const std::vector<std::vector<Sudo>> board) {
    // If the digit is already present in the column, then the placement is not valid
    for (const auto& index: INDICES) {
        if (board[rowIndex][index] == digit) {
            return false;
        }
    }
    return true;
}

bool SudokuRow::satisfy(std::vector<std::vector<Sudo>> board) {
    // The board satisfies the constraint if all rows do not contain duplicate digits
    for (const auto& rowIndex: INDICES) {
        for (const auto& digit: SUDO_DIGITS) {
            int8_t count = 0;
            for (const auto& columnIndex: INDICES) {
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
