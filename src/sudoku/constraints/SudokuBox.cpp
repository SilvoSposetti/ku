#include "SudokuBox.h"
#include <cmath>

ConstraintType SudokuBox::getType() {
    return ConstraintType::SUDOKU_BOX;
}

std::string SudokuBox::getName() {
    return "Sudoku-Box";
}

std::string SudokuBox::getDescription() {
    return "3x3 boxes contain all the digits from 1 to 9 exactly once.";
}

std::string SudokuBox::getSvgGroup() {
    return "";
}

bool SudokuBox::validatePlacement(const Sudo digit, const int8_t rowIndex, const int8_t columnIndex,
                                  const std::vector<std::vector<Sudo>> board) {
    // Validate a hypothetical placement by checking if the digit already exists in the box relevant for the given coordinates
    const int8_t boxIndex = getBoxIndex(rowIndex, columnIndex);
    std::vector<std::pair<int8_t, int8_t>> boxIndexPairs = getBoxIndices()[boxIndex];
    for (const auto& pair: boxIndexPairs) {
        if (board[pair.first][pair.second] == digit) {
            return false;
        }
    }
    return true;
}


bool SudokuBox::satisfy(std::vector<std::vector<Sudo>> board) {
    // The board satisfies the constraint if all boxes do not contain duplicate digits
    std::vector<std::vector<std::pair<int8_t, int8_t>>> boxesIndices = getBoxIndices();
    for (const auto& d: SUDO_DIGITS) {
        for (const auto& boxIndexPairs: boxesIndices) {
            int8_t count = 0;
            for (const auto& pair: boxIndexPairs) {
                if (board[pair.first][pair.second] == d) {
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

int8_t SudokuBox::getBoxIndex(const int8_t rowIndex, const int8_t columnIndex) {
    // Use integer division to floor results
    return columnIndex / 3 + 3 * (rowIndex / 3);
}

std::vector<std::vector<std::pair<int8_t, int8_t>>> SudokuBox::getBoxIndices() {
    std::vector<std::vector<std::pair<int8_t, int8_t>>> allBoxIndexPairs;
    for (int8_t boxIndexRow = 0; boxIndexRow < 3; boxIndexRow++) {
        for (int8_t boxIndexColumn = 0; boxIndexColumn < 3; boxIndexColumn++) {
            std::vector<std::pair<int8_t, int8_t>> singleBoxIndexPairs;
            for (int8_t i = 0; i < 3; i++) {
                for (int8_t j = 0; j < 3; j++) {
                    singleBoxIndexPairs.emplace_back(std::make_pair(boxIndexRow * 3 + i, boxIndexColumn * 3 + j));
                }
            }
            allBoxIndexPairs.emplace_back(singleBoxIndexPairs);
        }
    }
    return allBoxIndexPairs;
}
