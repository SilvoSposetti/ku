#include "SudokuBox.h"
#include <cmath>

ConstraintType SudokuBox::getType() const {
    return ConstraintType::SUDOKU_BOX;
}

std::string SudokuBox::getName() const {
    return "Sudoku-Box";
}

std::string SudokuBox::getDescription() const {
    return "3x3 boxes contain all the digits from 1 to 9 exactly once.";
}

std::string SudokuBox::getSvgGroup() const {
    return "";
}

bool SudokuBox::validatePlacement(const Sudo digit,
                                  const int8_t rowIndex,
                                  const int8_t columnIndex,
                                  const std::vector<std::vector<Sudo>> board) const {
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


bool SudokuBox::satisfy(std::vector<std::vector<Sudo>> board) const {
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
    return static_cast<int8_t>(columnIndex / 3 + 3 * (rowIndex / 3));
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

int32_t SudokuBox::getDLXConstraintColumnsAmount() const {
    return 9 * 9; // 9(boxes), 9(possible digits in each box)
}

bool SudokuBox::getDLXConstraint(const int8_t boardRow,
                                 const int8_t boardColumn,
                                 const int32_t columnId,
                                 const Sudo possibleDigit,
                                 const Sudo actualDigit) const {

    const int32_t matrixRow = columnId / (MAX_INDEX + 1);
    const int32_t matrixColumn = columnId % (MAX_INDEX + 1);

    if (actualDigit == Sudo::NONE) {
        return matrixRow == getBoxIndex(boardRow, boardColumn) &&
               matrixColumn == static_cast<int32_t>(possibleDigit) - 1;
    }
    return actualDigit == possibleDigit &&
           matrixRow == getBoxIndex(boardRow, boardColumn) &&
           matrixColumn == static_cast<int32_t>(possibleDigit) - 1;
}


