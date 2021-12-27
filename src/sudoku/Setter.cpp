#include "Setter.h"

#include "Validator.h"

std::unique_ptr<Board> Setter::generate(const int8_t totalDigits,
                                        const SetterType setterType,
                                        const std::vector<std::unique_ptr<AbstractConstraint>>& constraints) {

    std::vector<std::vector<Sudo>> randomBoard = Setter::createRandomBoard(constraints);

    // TODO: Add differentiation with setterType argument
    std::vector<std::vector<bool>> givenMask = randomGivenMask(totalDigits);
    std::unique_ptr<Board> newBoard = std::make_unique<Board>(randomBoard, givenMask);

    return newBoard;
}

std::vector<std::vector<Sudo>> Setter::createRandomBoard(const std::vector<std::unique_ptr<AbstractConstraint>>& constraints) {
    std::vector<std::vector<Sudo>> newBoard = emptyField();
    if (Setter::createRandomRecursive(0, 0, newBoard, constraints)) {
        return newBoard;
    } else {
        static_assert(true, "Was not able to generate a new board with the given constraints");
    }
    return emptyField();
}

bool Setter::createRandomRecursive(int8_t rowIndex,
                                   int8_t columnIndex,
                                   std::vector<std::vector<Sudo>>& board,
                                   const std::vector<std::unique_ptr<AbstractConstraint>>& constraints) {

    // Reached end of the board, so found solution must be valid
    if (rowIndex > 8) {
        return true;
    }
    // Try out all possible digits in random order
    std::vector<Sudo> randomDigits = randomShuffle(SUDO_DIGITS);
    for (const auto& digit: randomDigits) {
        // Check if the current digit can be placed here without breaking any constraint
        if (Validator::validateMove(rowIndex, columnIndex, digit, board, constraints)) {
            // Store digit's place
            const int8_t k = rowIndex;
            const int8_t l = columnIndex;
            board[k][l] = digit; // Try and set the current digit in the board
            // Go forward in the grid
            columnIndex++;
            if (columnIndex > 8) {
                rowIndex++;
                columnIndex = 0;
            }
            // If the recursive call returns true, a valid solution was found
            if (Setter::createRandomRecursive(rowIndex, columnIndex, board, constraints)) {
                return true;
            }
            // Otherwise, the digit just placed leads to conflict
            board[k][l] = Sudo::NONE;
            // Thus, go back one digit in the board
            columnIndex--;
            if (columnIndex < 0) {
                rowIndex--;
                columnIndex = 8;
            }
        }
    }

    return false;
}

std::vector<std::vector<bool>> Setter::randomGivenMask(const int8_t totalDigits) {
    std::vector<std::vector<bool>> mask = emptyGivenMask();
    int8_t digits = 81;
    while (digits != totalDigits) {
        const int8_t randomRow = randomUniform();
        const int8_t randomColumn = randomUniform();
        if (mask[randomRow][randomColumn]) {
            mask[randomRow][randomColumn] = !mask[randomRow][randomColumn];
            digits--;
        }
    }

    return mask;
}
