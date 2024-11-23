#include "Validator.hpp"

#include <iostream>

bool Validator::checkSolution(const std::vector<std::vector<Sudo::Digit>>& board,
                              const std::vector<std::unique_ptr<AbstractConstraint>>& constraints) {
  // There is no blank cell
  for (const auto& rowIndex : Sudo::INDICES) {
    for (const auto& columnIndex : Sudo::INDICES) {
      if (board[rowIndex][columnIndex] == Sudo::Digit::NONE) {
        std::cout << "ERROR: Blank cell found in the solution!" << std::endl;
        return false;
      }
    }
  }

  // The board satisfies all constraints
  for (const auto& constraint : constraints) {
    if (!constraint->satisfy(board)) {
      std::cout << "ERROR: Board does not satisfy constraint: " + constraint->getName() << std::endl;
      return false;
    }
  }
  return true;
}
