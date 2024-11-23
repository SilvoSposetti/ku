#include "KuTestArguments.hpp"
#include "Sudoku.hpp"
#include "constraints/ConstraintType.hpp"
#include "solver/Solver.hpp"

#include <doctest.h>

static std::vector<std::vector<Sudo::Digit>> generateBoard(ConstraintType constraintType) {
  const auto constraints =
      Sudoku::getConstraintsList(ConstraintType::SUDOKU_CELL | ConstraintType::SUDOKU_COLUMN |
                                 ConstraintType::SUDOKU_ROW | ConstraintType::SUDOKU_BOX | constraintType);
  return Solver::createNewBoard(constraints, KuTestArguments::seed);
};

TEST_SUITE("Classic Sudoku Base") {

  TEST_CASE("Classic Sudoku") {
    const auto board = generateBoard(ConstraintType::NONE);
    CHECK(board != Sudo::emptyField());
  }

  TEST_CASE("Classic Sudoku Base: Positive Diagonal") {
    const auto board = generateBoard(ConstraintType::POSITIVE_DIAGONAL);
    CHECK(board != Sudo::emptyField());
  }

  TEST_CASE("Classic Sudoku Base: Positive Diagonal Even") {
    const auto board = generateBoard(ConstraintType::POSITIVE_DIAGONAL_EVEN);
    CHECK(board != Sudo::emptyField());
  }

  TEST_CASE("Classic Sudoku Base: Positive Diagonal Odd") {
    const auto board = generateBoard(ConstraintType::POSITIVE_DIAGONAL_ODD);
    CHECK(board != Sudo::emptyField());
  }

  TEST_CASE("Classic Sudoku Base: Negative Diagonal") {
    const auto board = generateBoard(ConstraintType::NEGATIVE_DIAGONAL);
    CHECK(board != Sudo::emptyField());
  }

  TEST_CASE("Classic Sudoku Base: Negative Diagonal Even") {
    const auto board = generateBoard(ConstraintType::NEGATIVE_DIAGONAL_EVEN);
    CHECK(board != Sudo::emptyField());
  }

  TEST_CASE("Classic Sudoku Base: Negative Diagonal Odd") {
    const auto board = generateBoard(ConstraintType::NEGATIVE_DIAGONAL_ODD);
    CHECK(board != Sudo::emptyField());
  }

  TEST_CASE("Classic Sudoku Base: Anti King") {
    const auto board = generateBoard(ConstraintType::ANTI_KING);
    CHECK(board != Sudo::emptyField());
  }

  TEST_CASE("Classic Sudoku Base: Anti King Torus") {
    const auto board = generateBoard(ConstraintType::ANTI_KING_TORUS);
    CHECK(board != Sudo::emptyField());
  }

  TEST_CASE("Classic Sudoku Base: Anti Knight") {
    const auto board = generateBoard(ConstraintType::ANTI_KNIGHT);
    CHECK(board != Sudo::emptyField());
  }

  TEST_CASE("Classic Sudoku Base: Anti Knight Torus") {
    const auto board = generateBoard(ConstraintType::ANTI_KNIGHT_TORUS);
    CHECK(board != Sudo::emptyField());
  }

  TEST_CASE("Classic Sudoku Base: Disjoint Boxes") {
    const auto board = generateBoard(ConstraintType::DISJOINT_BOXES);
    CHECK(board != Sudo::emptyField());
  }

  TEST_CASE("Classic Sudoku Base: Asterisk") {
    const auto board = generateBoard(ConstraintType::ASTERISK);
    CHECK(board != Sudo::emptyField());
  }
}