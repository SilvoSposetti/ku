
#include "Sudoku.h"
#include "solver/Solver.h"

#include <doctest.h>

TEST_CASE("Generate Solution") {

  const auto generateBoard = [&](ConstraintType constraintType) -> std::vector<std::vector<Sudo>> {
    const auto constraints = Sudoku::getConstraintsList(constraintType);
    std::shared_ptr<RandomGenerator> generator = std::make_shared<RandomGenerator>(0);
    return Solver::createNewBoard(constraints, generator);
  };

  SUBCASE("Sudoku Cell") {
    const auto board = generateBoard(ConstraintType::SUDOKU_CELL);
    CHECK(board != emptyField());
  }
  SUBCASE("Sudoku Row") {
    const auto board = generateBoard(ConstraintType::SUDOKU_ROW);
    CHECK(board != emptyField());
  }
  SUBCASE("Sudoku Column") {
    const auto board = generateBoard(ConstraintType::SUDOKU_COLUMN);
    CHECK(board != emptyField());
  }
  SUBCASE("Sudoku Box") {
    const auto board = generateBoard(ConstraintType::SUDOKU_BOX);
    CHECK(board != emptyField());
  }
  SUBCASE("Positive Diagonal") {
    const auto board = generateBoard(ConstraintType::POSITIVE_DIAGONAL);
    CHECK(board != emptyField());
  }
  SUBCASE("Positive Diagonal Even") {
    const auto board = generateBoard(ConstraintType::POSITIVE_DIAGONAL_EVEN);
    CHECK(board != emptyField());
  }
  SUBCASE("Positive Diagonal Odd") {
    const auto board = generateBoard(ConstraintType::POSITIVE_DIAGONAL_ODD);
    CHECK(board != emptyField());
  }
  SUBCASE("Negative Diagonal") {
    const auto board = generateBoard(ConstraintType::NEGATIVE_DIAGONAL);
    CHECK(board != emptyField());
  }
  SUBCASE("Negative Diagonal Even") {
    const auto board = generateBoard(ConstraintType::NEGATIVE_DIAGONAL_EVEN);
    CHECK(board != emptyField());
  }
  SUBCASE("Negative Diagonal Odd") {
    const auto board = generateBoard(ConstraintType::NEGATIVE_DIAGONAL_ODD);
    CHECK(board != emptyField());
  }
  SUBCASE("Anti King") {
    const auto board = generateBoard(ConstraintType::ANTI_KING);
    CHECK(board != emptyField());
  }
  SUBCASE("Anti King Torus") {
    const auto board = generateBoard(ConstraintType::ANTI_KING_TORUS);
    CHECK(board != emptyField());
  }
  SUBCASE("Anti Knight") {
    const auto board = generateBoard(ConstraintType::ANTI_KNIGHT);
    CHECK(board != emptyField());
  }
  SUBCASE("Anti Knight Torus") {
    const auto board = generateBoard(ConstraintType::ANTI_KNIGHT_TORUS);
    CHECK(board != emptyField());
  }
  SUBCASE("Disjoint Boxes") {
    const auto board = generateBoard(ConstraintType::DISJOINT_BOXES);
    CHECK(board != emptyField());
  }
  SUBCASE("Asterisk") {
    const auto board = generateBoard(ConstraintType::ASTERISK);
    CHECK(board != emptyField());
  }
}
