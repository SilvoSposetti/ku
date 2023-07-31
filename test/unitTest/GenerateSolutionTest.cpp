
#include "Sudoku.h"
#include "solver/Solver.h"

#include <doctest.h>

static std::vector<std::vector<Sudo::Digit>> generateBoard(ConstraintType constraintType) {
  const auto constraints = Sudoku::getConstraintsList(constraintType);
  std::shared_ptr<RandomGenerator> generator = std::make_shared<RandomGenerator>(0);
  return Solver::createNewBoard(constraints, generator);
};

TEST_CASE("Generate Solution") {
  SUBCASE("Sudoku Cell") {
    const auto board = generateBoard(ConstraintType::SUDOKU_CELL);
    CHECK(board != Sudo::emptyField());
  }
  SUBCASE("Sudoku Row") {
    const auto board = generateBoard(ConstraintType::SUDOKU_ROW);
    CHECK(board != Sudo::emptyField());
  }
  SUBCASE("Sudoku Column") {
    const auto board = generateBoard(ConstraintType::SUDOKU_COLUMN);
    CHECK(board != Sudo::emptyField());
  }
  SUBCASE("Sudoku Box") {
    const auto board = generateBoard(ConstraintType::SUDOKU_BOX);
    CHECK(board != Sudo::emptyField());
  }
  SUBCASE("Positive Diagonal") {
    const auto board = generateBoard(ConstraintType::POSITIVE_DIAGONAL);
    CHECK(board != Sudo::emptyField());
  }
  SUBCASE("Positive Diagonal Even") {
    const auto board = generateBoard(ConstraintType::POSITIVE_DIAGONAL_EVEN);
    CHECK(board != Sudo::emptyField());
  }
  SUBCASE("Positive Diagonal Odd") {
    const auto board = generateBoard(ConstraintType::POSITIVE_DIAGONAL_ODD);
    CHECK(board != Sudo::emptyField());
  }
  SUBCASE("Negative Diagonal") {
    const auto board = generateBoard(ConstraintType::NEGATIVE_DIAGONAL);
    CHECK(board != Sudo::emptyField());
  }
  SUBCASE("Negative Diagonal Even") {
    const auto board = generateBoard(ConstraintType::NEGATIVE_DIAGONAL_EVEN);
    CHECK(board != Sudo::emptyField());
  }
  SUBCASE("Negative Diagonal Odd") {
    const auto board = generateBoard(ConstraintType::NEGATIVE_DIAGONAL_ODD);
    CHECK(board != Sudo::emptyField());
  }
  SUBCASE("Anti King") {
    const auto board = generateBoard(ConstraintType::ANTI_KING);
    CHECK(board != Sudo::emptyField());
  }
  SUBCASE("Anti King Torus") {
    const auto board = generateBoard(ConstraintType::ANTI_KING_TORUS);
    CHECK(board != Sudo::emptyField());
  }
  SUBCASE("Anti Knight") {
    const auto board = generateBoard(ConstraintType::ANTI_KNIGHT);
    CHECK(board != Sudo::emptyField());
  }
  SUBCASE("Anti Knight Torus") {
    const auto board = generateBoard(ConstraintType::ANTI_KNIGHT_TORUS);
    CHECK(board != Sudo::emptyField());
  }
  SUBCASE("Disjoint Boxes") {
    const auto board = generateBoard(ConstraintType::DISJOINT_BOXES);
    CHECK(board != Sudo::emptyField());
  }
  SUBCASE("Asterisk") {
    const auto board = generateBoard(ConstraintType::ASTERISK);
    CHECK(board != Sudo::emptyField());
  }
}
