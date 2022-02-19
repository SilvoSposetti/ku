#include "Setter.h"
#include "solver/Solver.h"

std::unique_ptr<Board> Setter::generate(int8_t totalDigits,
                                        SymmetryType symmetryType,
                                        const std::vector<std::unique_ptr<AbstractConstraint>>& constraints) {
                                        
    // Timer timer;
    // Create a new board
    // std::vector<std::vector<Sudo>> randomBoard = Solver::createBoard(constraints, SolverType::Naive);
    std::vector<std::vector<Sudo>> randomBoard = Solver::createBoard(constraints, SolverType::DLX);
    // timer.printElapsed("Creating Random Board");

    // Create given mask
    std::vector<std::vector<bool>> givenMask;
    if (symmetryType == SymmetryType::ONE_DIAGONAL_MIRROR) givenMask = diagonalMirrorMask(totalDigits);
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

    // TODO: Check given mask according to SymmetryType
    checkGivenMask(givenMask, totalDigits);

    std::unique_ptr<Board> newBoard = std::make_unique<Board>(randomBoard, givenMask);

    return newBoard;
}


void Setter::checkGivenMask(const std::vector<std::vector<bool>>& mask, int8_t totalDigits) {
    int8_t counter = 0;
    for (int8_t i = MIN_INDEX; i <= MAX_INDEX; i++) {
        for (int8_t j = MIN_INDEX; j <= MAX_INDEX; j++) {
            if (mask[i][j]) counter++;
        }
    }
    if (counter != totalDigits) {
        std::cout << "ERROR: Mask does not contain the correct amount of digits" << std::endl;
    }
}

std::vector<std::vector<bool>> Setter::randomMask(int8_t totalDigits) {
    std::vector<std::vector<bool>> mask = fullGivenMask();
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

inline bool Setter::isOnMainDiagonal(int8_t rowIndex, int8_t columnIndex) {
    return rowIndex == columnIndex;
}

inline bool Setter::isOnCenter(int8_t rowIndex, int8_t columnIndex) {
    return rowIndex == MID_INDEX && columnIndex == MID_INDEX;
}

std::vector<std::vector<bool>> Setter::diagonalMirrorMask(int8_t totalDigits) {
    std::vector<std::vector<bool>> mask = fullGivenMask();
    int8_t digitsToRemove = TOTAL_DIGITS - totalDigits;
    // "Diagonal" digits are removed once at the beginning, then in pairs
    int8_t diagonalDigitsRemaining = 9;
    // "Other" ("Non-Diagonal") are always removed in pairs for symmetry
    int8_t otherDigitsRemaining = 72;

    if (digitsToRemove % 2 != 0) {
        const int8_t randomIndex = randomUniform();
        mask[randomIndex][randomIndex] = false;
        digitsToRemove--;
        diagonalDigitsRemaining--;
    }

    while (digitsToRemove > 0) {
        // Randomly decide if two digits need to be removed either from the diagonal or from the other ones
        const int8_t random = randomUniform(2, diagonalDigitsRemaining + otherDigitsRemaining);
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

std::vector<std::vector<bool>> Setter::diagonalRotationMask(int8_t totalDigits) {
    std::vector<std::vector<bool>> mask = fullGivenMask();
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





