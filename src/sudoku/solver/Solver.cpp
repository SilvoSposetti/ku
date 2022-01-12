#include "Solver.h"
#include "../Validator.h"
#include "../../utilities/Utilities.h"

std::vector<std::vector<Sudo>>
Solver::createBoard(const std::vector<std::unique_ptr<AbstractConstraint>>& constraints, const SolverType solverType) {
    std::vector<std::vector<Sudo>> newBoard = emptyField();
    std::vector<std::vector<bool>> givenMask = emptyGivenMask();

    bool created = false;
    if (solverType == SolverType::BruteForce) {
        created = Solver::randomBruteForceRecursive(0, 0, newBoard, givenMask, constraints);
    } else if (solverType == SolverType::DLX) {
        created = Solver::randomDLX(0, 0, newBoard, givenMask, constraints);
    }

    if (!created) {
        std::cout << "ERROR: Was not able to generate a new board with the given constraints" << std::endl;
    }
    if (!Validator::checkSolution(newBoard, constraints)) {
        std::cout << "ERROR: Solution created does not satisfy all constraints" << std::endl;
    }
    return newBoard;
}

bool Solver::randomBruteForceRecursive(int8_t rowIndex,
                                       int8_t columnIndex,
                                       std::vector<std::vector<Sudo>>& board,
                                       const std::vector<std::vector<bool>>& givenMask,
                                       const std::vector<std::unique_ptr<AbstractConstraint>>& constraints) {
    // Reached end of the board, so found solution must be valid
    if (rowIndex > 8) {
        return true;
    }
    // Current digit can be modified if it's not part of the given mask
    if (!givenMask[rowIndex][columnIndex]) {
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
                if (Solver::randomBruteForceRecursive(rowIndex, columnIndex, board, givenMask, constraints)) {
                    return true;
                }
                // Otherwise, the digit just placed leads to a conflict
                board[k][l] = Sudo::NONE;
                // Thus, go back one digit in the board
                columnIndex--;
                if (columnIndex < 0) {
                    rowIndex--;
                    columnIndex = 8;
                }
            }
        }
    } else {
        // The digit cannot be modified, go to the next one
        columnIndex++;
        if (columnIndex > 8) {
            rowIndex++;
            columnIndex = 0;
        }
        return Solver::randomBruteForceRecursive(rowIndex, columnIndex, board, givenMask, constraints);
    }
    return false;
}


bool Solver::randomDLX(int8_t rowIndex, int8_t columnIndex, std::vector<std::vector<Sudo>>& board,
                       const std::vector<std::vector<bool>>& givenMask,
                       const std::vector<std::unique_ptr<AbstractConstraint>>& constraints) {
    // Create matrix
    constexpr int32_t totalRows = 9 * 9 * 9; //729 (81 cells, 9 possible digits for each cell)

    std::vector<std::vector<bool>> M;
    // Default "Single digit in each cell constraint"
    std::cout << "M:                    |";
    for (const auto& constraint: constraints) {
        std::string str = " " + constraint->getName();
        while (str.size() < constraint->getDLXConstraintColumnsAmount() + 2) {
            str += " ";
        }
        std::cout << str << "|";
    }

//    board[0][0] = Sudo::C;
//    board[0][2] = Sudo::F;
//    board[8][5] = Sudo::H;
    std::cout << std::endl;
    for (int8_t boardRow = 0; boardRow <= MAX_INDEX; boardRow++) {
        for (int8_t boardColumn = 0; boardColumn <= MAX_INDEX; boardColumn++) {
            for (const auto& possibleDigit: SUDO_DIGITS) {
                std::cout << "Cell [" << std::to_string(boardRow) << ", " << std::to_string(boardColumn)
                          << "] Digit: "
                          << std::to_string(static_cast<int8_t>(possibleDigit)) << "  | ";
                std::vector<bool> row;
                // TODO: maybe all rows in which actualDigit is not None and actualDigit != possibleDigit can be avoided
                for (const auto& constraint: constraints) {
                    int32_t totalConstraintColumns = constraint->getDLXConstraintColumnsAmount();
                    for (int32_t columnId = 0; columnId < totalConstraintColumns; columnId++) {
                        const bool value = constraint->getDLXConstraint(boardRow,
                                                                        boardColumn,
                                                                        columnId,
                                                                        possibleDigit,
                                                                        board[boardRow][boardColumn]);
                        std::cout << (value ? "■" : "□");
                        row.emplace_back(value);
                    }
                    std::cout << " | ";
                }
                std::cout << std::endl;
                M.emplace_back(row);
            }
        }
    }
    std::cout << "M(" << M.size() << "x" << M[0].size() << ")" << std::endl;
    std::cout << M.size() << " = 81(cells) * 9(possible digits for each cell)" << std::endl;
    std::cout << M[0].size() << " = " << constraints.size() << "(constraints) * 81(columns per constraint)"
              << std::endl;


    std::cout << std::endl;
    std::cout << std::endl;
    return false;
}
