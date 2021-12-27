#include "Setter.h"

#include "Validator.h"

std::unique_ptr<Board> Setter::generate(const int8_t totalDigits,
                                        const SymmetryType symmetryType,
                                        const std::vector<std::unique_ptr<AbstractConstraint>>& constraints) {

    std::vector<std::vector<Sudo>> randomBoard = Setter::createRandomBoard(constraints);

    std::vector<std::vector<bool>> givenMask;
    if (symmetryType == SymmetryType::RANDOM) givenMask = randomMask(totalDigits);
    else if (symmetryType == SymmetryType::ONE_DIAGONAL_MIRROR) givenMask = diagonalMirrorMask(totalDigits);
    else if (symmetryType == SymmetryType::ONE_DIAGONAL_ROTATION) givenMask = diagonalRotationMask(totalDigits);
//    else if (symmetryType == SymmetryType::TWO_DIAGONALS_MIRROR) givenMask = randomMask(totalDigits);
//    else if (symmetryType == SymmetryType::TWO_DIAGONALS_ROTATION) givenMask = randomMask(totalDigits);
//    else if (symmetryType == SymmetryType::ONE_AXIS_MIRROR) givenMask = randomMask(totalDigits);
//    else if (symmetryType == SymmetryType::ONE_AXIS_ROTATION) givenMask = randomMask(totalDigits);
//    else if (symmetryType == SymmetryType::TWO_AXES_MIRROR) givenMask = randomMask(totalDigits);
//    else if (symmetryType == SymmetryType::TWO_AXES_ROTATION) givenMask = randomMask(totalDigits);
    else givenMask = randomMask(totalDigits);
//    givenMask = diagonalMirrorMask(totalDigits);
//    givenMask = diagonalRotationMask(totalDigits);

    checkGivenMask(givenMask, totalDigits);

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

void Setter::checkGivenMask(const std::vector<std::vector<bool>>& mask, const int8_t totalDigits) {
    int8_t counter = 0;
    for (int8_t i = MIN_INDEX; i <= MAX_INDEX; i++) {
        for (int8_t j = MIN_INDEX; j <= MAX_INDEX; j++) {
            if (mask[i][j]) counter++;
        }
    }
    if (counter != totalDigits) {
        static_assert(true, "Mask does not contain the correct amount of digits");
    }
}

std::vector<std::vector<bool>> Setter::randomMask(const int8_t totalDigits) {
    std::vector<std::vector<bool>> mask = emptyGivenMask();
    int8_t digits = 81;
    while (digits != totalDigits) {
        const int8_t i = randomUniform();
        const int8_t j = randomUniform();
        if (mask[i][j]) {
            mask[i][j] = !mask[i][j];
            digits--;
        }
    }
    return mask;
}

inline bool Setter::isOnMainDiagonal(const int8_t rowIndex, int8_t columnIndex) {
    return rowIndex == columnIndex;
}

inline bool Setter::isOnCenter(const int8_t rowIndex, const int8_t columnIndex) {
    return rowIndex == MID_INDEX && columnIndex == MID_INDEX;
}

std::vector<std::vector<bool>> Setter::diagonalMirrorMask(const int8_t totalDigits) {
    std::vector<std::vector<bool>> mask = emptyGivenMask();
    int8_t digitsToRemove = TOTAL_DIGITS - totalDigits;
    int8_t diagonalDigitsRemaining = 9; // Removed as single digit first, then removed in pairs
    int8_t otherDigitsRemaining = 72; // Removed in pairs for symmetry

    if (digitsToRemove % 2 != 0) {
        const int8_t randomIndex = randomUniform();
        mask[randomIndex][randomIndex] = false;
        digitsToRemove--;
        diagonalDigitsRemaining--;
    }

    while (digitsToRemove > 0) {
        // Randomly decide if two digits need to be removed either from the diagonal or from the other ones
        const int8_t random = randomUniform(static_cast<uint8_t>(2), diagonalDigitsRemaining + otherDigitsRemaining);
        if (random < diagonalDigitsRemaining) {
            // Remove two digits from the diagonal
            const int8_t index1 = randomUniform();
            const int8_t index2 = randomUniform();
            if (index1 != index2) {
                if (mask[index1][index1] && mask[index2][index2]) {
                    mask[index1][index1] = false;
                    digitsToRemove--;
                    diagonalDigitsRemaining--;
                    mask[index2][index2] = false;
                    digitsToRemove--;
                    diagonalDigitsRemaining--;
                }
            }
        } else {
            // Remove two digits from the other digits
            const int8_t i = randomUniform();
            const int8_t j = randomUniform();
            if (!isOnMainDiagonal(i, j) && mask[i][j]) {
                mask[i][j] = false;
                digitsToRemove--;
                otherDigitsRemaining--;
                const int8_t k = j;
                const int8_t l = i;
                mask[k][l] = false;
                digitsToRemove--;
                otherDigitsRemaining--;
            }
        }
    }
    return mask;
}

std::vector<std::vector<bool>> Setter::diagonalRotationMask(const int8_t totalDigits) {
    std::vector<std::vector<bool>> mask = emptyGivenMask();
    int8_t digitsToRemove = TOTAL_DIGITS - totalDigits;

    // If amount of total digits to remove is not even: need to remove the center
    if (digitsToRemove % 2 != 0) {
        mask[4][4] = false;
        digitsToRemove--;
    }
    // Then, can remove digits two-by-to with diagonal rotation
    while (digitsToRemove > 0) {
        const int8_t i = randomUniform();
        const int8_t j = randomUniform();
        if (!isOnCenter(i, j) && digitsToRemove >= 2) {
            if (mask[i][j]) {
                mask[i][j] = false;
                digitsToRemove--;
                const int8_t k = 8 - i;
                const int8_t l = 8 - j;
                mask[k][l] = false;
                digitsToRemove--;
            }
        }

    }
    return mask;
}





