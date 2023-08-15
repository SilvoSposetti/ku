#include "Sudoku.h"
#include "doctest.h"
#include "solver/Solver.h"

static std::vector<std::vector<Sudo::Digit>> generateBoard(ConstraintType constraintType) {
  const auto constraints = Sudoku::getConstraintsList(constraintType);
  return Solver::createNewBoard(constraints, std::nullopt);
};

TEST_SUITE("Single Constraint") {

  TEST_CASE("Single Constraint: Sudoku Cell") {
    const auto board = generateBoard(ConstraintType::SUDOKU_CELL);
    CHECK(board != Sudo::emptyField());
  }

  TEST_CASE("Single Constraint: Sudoku Row") {
    const auto board = generateBoard(ConstraintType::SUDOKU_ROW);
    CHECK(board != Sudo::emptyField());
  }

  TEST_CASE("Single Constraint: Sudoku Column") {
    const auto board = generateBoard(ConstraintType::SUDOKU_COLUMN);
    CHECK(board != Sudo::emptyField());
  }

  TEST_CASE("Single Constraint: Sudoku Box") {
    const auto board = generateBoard(ConstraintType::SUDOKU_BOX);
    CHECK(board != Sudo::emptyField());
  }

  TEST_CASE("Single Constraint: Positive Diagonal") {
    const auto board = generateBoard(ConstraintType::POSITIVE_DIAGONAL);
    CHECK(board != Sudo::emptyField());
  }

  TEST_CASE("Single Constraint: Positive Diagonal Even") {
    const auto board = generateBoard(ConstraintType::POSITIVE_DIAGONAL_EVEN);
    CHECK(board != Sudo::emptyField());
  }

  TEST_CASE("Single Constraint: Positive Diagonal Odd") {
    const auto board = generateBoard(ConstraintType::POSITIVE_DIAGONAL_ODD);
    CHECK(board != Sudo::emptyField());
  }

  TEST_CASE("Single Constraint: Negative Diagonal") {
    const auto board = generateBoard(ConstraintType::NEGATIVE_DIAGONAL);
    CHECK(board != Sudo::emptyField());
  }

  TEST_CASE("Single Constraint: Negative Diagonal Even") {
    const auto board = generateBoard(ConstraintType::NEGATIVE_DIAGONAL_EVEN);
    CHECK(board != Sudo::emptyField());
  }

  TEST_CASE("Single Constraint: Negative Diagonal Odd") {
    const auto board = generateBoard(ConstraintType::NEGATIVE_DIAGONAL_ODD);
    CHECK(board != Sudo::emptyField());
  }

  TEST_CASE("Single Constraint: Anti King") {
    const auto board = generateBoard(ConstraintType::ANTI_KING);
    CHECK(board != Sudo::emptyField());
  }

  TEST_CASE("Single Constraint: Anti King Torus") {
    const auto board = generateBoard(ConstraintType::ANTI_KING_TORUS);
    CHECK(board != Sudo::emptyField());
  }

  TEST_CASE("Single Constraint: Anti Knight") {
    const auto board = generateBoard(ConstraintType::ANTI_KNIGHT);
    CHECK(board != Sudo::emptyField());
  }

  TEST_CASE("Single Constraint: Anti Knight Torus") {
    const auto board = generateBoard(ConstraintType::ANTI_KNIGHT_TORUS);
    CHECK(board != Sudo::emptyField());
  }

  TEST_CASE("Single Constraint: Disjoint Boxes") {
    const auto board = generateBoard(ConstraintType::DISJOINT_BOXES);
    CHECK(board != Sudo::emptyField());
  }

  TEST_CASE("Single Constraint: Asterisk") {
    const auto board = generateBoard(ConstraintType::ASTERISK);
    CHECK(board != Sudo::emptyField());
  }
}