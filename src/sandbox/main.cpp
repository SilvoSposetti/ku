#include "Sudoku.h"
#include "randomGenerator/RandomGenerator.h"
#include "utilities/Utilities.h"

#include <iostream>
int main() {
  // ConstraintType constraints = Constraint::getSudokuConstraints();
  ConstraintType constraints = ConstraintType::SUDOKU_CELL;
  constraints = constraints | ConstraintType::SUDOKU_ROW;
  constraints = constraints | ConstraintType::SUDOKU_COLUMN;
  constraints = constraints | ConstraintType::SUDOKU_BOX;
  // constraints = constraints | ConstraintType::POSITIVE_DIAGONAL;
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

  const std::filesystem::path location = std::filesystem::path(OUT_DIR) / "sandbox";
  for (int32_t i = 0; i < 1; ++i) {
    Sudoku sudoku("Sandbox_" + std::to_string(i + 1), constraints, SymmetryType::RANDOM, 40);
    sudoku.printInfo();
    sudoku.printBoard();
    sudoku.exportToSvg(location);
    sudoku.exportDlxMatrixToSvg(location);
  }

  return EXIT_SUCCESS;
}