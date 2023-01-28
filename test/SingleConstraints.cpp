
#include "Sudoku.h"

#include <doctest.h>

TEST_CASE("Single constraint, no missing digits") {
  SUBCASE("Sudoku-Cell") {
    Sudoku sudoku("Sudoku-Cell", ConstraintType::SUDOKU_CELL);
    CHECK(sudoku.verify());
  }
  SUBCASE("Sudoku-Box") {
    Sudoku sudoku("Sudoku-Box", ConstraintType::SUDOKU_BOX);
    CHECK(sudoku.verify());
  }
  SUBCASE("Sudoku-Row") {
    Sudoku sudoku("Sudoku-Row", ConstraintType::SUDOKU_ROW);
    CHECK(sudoku.verify());
  }
  SUBCASE("Sudoku-Column") {
    Sudoku sudoku("Sudoku-Column", ConstraintType::SUDOKU_COLUMN);
    CHECK(sudoku.verify());
  }
}
