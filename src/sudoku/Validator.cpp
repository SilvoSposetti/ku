#include "Validator.h"

bool Validator::checkSolution(const std::vector<std::vector<Sudo>>& board,
                              const std::vector<std::unique_ptr<AbstractConstraint>>& constraints) {
    // there is no blank cell
    for (const auto& rowIndex: INDICES) {
        for (const auto& columnIndex: INDICES) {
            if (board[rowIndex][columnIndex] == Sudo::NONE) {
                std::cout << "ERROR: Blank cell found in the solution!" << std::endl;
                return false;
            }
        }
    }
    return Validator::isBoardValid(board, constraints);
}

bool Validator::validateMove(const int8_t rowIndex,
                             const int8_t columnIndex,
                             const Sudo digit,
                             const std::vector<std::vector<Sudo>>& field,
                             const std::vector<std::unique_ptr<AbstractConstraint>>& constraints) {
    for (const auto& constraint: constraints) {
        if (!constraint->validatePlacement(digit, rowIndex, columnIndex, field)) {
            return false;
        }
    }
    return true;
}

bool Validator::isBoardValid(const std::vector<std::vector<Sudo>>& board,
                             const std::vector<std::unique_ptr<AbstractConstraint>>& constraints) {
    for (const auto& constraint: constraints) {
        if (!constraint->satisfy(board)) {
            std::cout << "ERROR: Board does not satisfy constraint: " + constraint->getName() << std::endl;
            return false;
        }
    }
    return true;
}
