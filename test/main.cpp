#include <cstdlib>

#include "Sudoku.h"

int main() {
  ConstraintType constraints = ConstraintType::NONE;
  constraints = constraints | ConstraintType::SUDOKU_ROW;
  constraints = constraints | ConstraintType::SUDOKU_COLUMN;
  constraints = constraints | ConstraintType::SUDOKU_BOX;
  // return EXIT_FAILURE;
  return EXIT_SUCCESS;
}