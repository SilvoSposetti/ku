#include "Sudoku.h"
#include "utilities/Utilities.h"

#include <iostream>
int main() {
  ConstraintType constraints = ConstraintType::NONE;
  constraints = constraints | ConstraintType::SUDOKU_ROW;
  constraints = constraints | ConstraintType::SUDOKU_COLUMN;
  constraints = constraints | ConstraintType::SUDOKU_BOX;
  constraints = constraints | ConstraintType::POSITIVE_DIAGONAL;
  // constraints = constraints | ConstraintType::POSITIVE_DIAGONAL_EVEN;
  // constraints = constraints | ConstraintType::POSITIVE_DIAGONAL_ODD;
  // constraints = constraints | ConstraintType::NEGATIVE_DIAGONAL;
  // constraints = constraints | ConstraintType::NEGATIVE_DIAGONAL_EVEN;
  // constraints = constraints | ConstraintType::NEGATIVE_DIAGONAL_ODD;
  // constraints = constraints | ConstraintType::ANTI_KING;
  // constraints = constraints | ConstraintType::ANTI_KING_TORUS;
  // constraints = constraints | ConstraintType::ANTI_KNIGHT;
  // constraints = constraints | ConstraintType::ANTI_KNIGHT_TORUS;
  // constraints = constraints | ConstraintType::DISJOINT_BOXES;
  // constraints = constraints | ConstraintType::ASTERISK;

  for (int32_t i = 0; i < 1; ++i) {
    const SymmetryType symmetryType = static_cast<SymmetryType>(i % static_cast<int32_t>(SymmetryType::AMOUNT));
    std::unique_ptr<Sudoku> sudoku =
        std::make_unique<Sudoku>("Sudoku " + std::to_string(i + 1), 60, constraints, symmetryType);
  }

  return EXIT_SUCCESS;
}