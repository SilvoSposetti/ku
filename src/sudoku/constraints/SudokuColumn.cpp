#include "SudokuColumn.h"

ConstraintType SudokuColumn::getType() {
    return ConstraintType::SUDOKU_BOX;
}

std::string SudokuColumn::getName() {
    return "Sudoku-Column";
}

std::string SudokuColumn::getDescription() {
    return "1x9 columns contain all the digits from 1 to 9 exactly once.";
}

std::string SudokuColumn::getSvgGroup() {
    return std::string();
}

bool SudokuColumn::validatePlacement(const Sudo digit, const int8_t rowIndex, const int8_t columnIndex, const std::vector<std::vector<Sudo>> board) {
    // If the digit is already present in the given row, then the placement is not valid
    for (const auto& index: INDICES) {
        if (board[index][columnIndex] == digit) {
            return false;
        }
    }
    return true;
}

bool SudokuColumn::satisfy(std::vector<std::vector<Sudo>> board) {
    // The board satisfies the constraint if all columns do not contain duplicate digits
    for (const auto& columnIndex: INDICES) {
        for (const auto& digit: SUDO_DIGITS) {
            int8_t count = 0;
            for (const auto& rowIndex: INDICES) {
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
