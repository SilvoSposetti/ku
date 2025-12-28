#include "KuTestArguments.hpp"
#include "puzzles/Puzzle.hpp"

#include <doctest.h>

TEST_SUITE("Single Constraint") {

  constexpr auto sudokuSpace = PuzzleSpace{9, 9, 9};
  constexpr auto emptyGrid = Grid<sudokuSpace>{};

  const auto check = []<PuzzleSpace space>(const std::string& name, ConstraintType constraints) {
    const auto puzzle = Puzzle<sudokuSpace>(name, emptyGrid, constraints, KuTestArguments::seed);
    CHECK_NE(puzzle.solution, emptyGrid);
  };

  TEST_CASE("Single Constraint: Sudoku Cell") {
    check.operator()<sudokuSpace>("Single Constraint: Sudoku Cell", ConstraintType::SUDOKU_CELL);
  }

  TEST_CASE("Single Constraint: Sudoku Row") {
    check.operator()<sudokuSpace>("Single Constraint: Sudoku Row", ConstraintType::EXACT_ROW);
  }

  TEST_CASE("Single Constraint: Sudoku Column") {
    check.operator()<sudokuSpace>("Single Constraint: Sudoku Column", ConstraintType::EXACT_COLUMN);
  }

  TEST_CASE("Single Constraint: Sudoku Box") {
    check.operator()<sudokuSpace>("Single Constraint: Sudoku Box", ConstraintType::EXACT_3x3_BOXES);
  }

  TEST_CASE("Single Constraint: Positive Diagonal") {
    check.operator()<sudokuSpace>("Single Constraint: Positive Diagonal", ConstraintType::POSITIVE_DIAGONAL);
  }

  TEST_CASE("Single Constraint: Positive Diagonal Even") {
    check.operator()<sudokuSpace>("Single Constraint: Positive Diagonal Even", ConstraintType::POSITIVE_DIAGONAL_EVEN);
  }

  TEST_CASE("Single Constraint: Positive Diagonal Odd") {
    check.operator()<sudokuSpace>("Single Constraint: Positive Diagonal Odd", ConstraintType::POSITIVE_DIAGONAL_ODD);
  }

  TEST_CASE("Single Constraint: Negative Diagonal") {
    check.operator()<sudokuSpace>("Single Constraint: Negative Diagonal", ConstraintType::NEGATIVE_DIAGONAL);
  }

  TEST_CASE("Single Constraint: Negative Diagonal Even") {
    check.operator()<sudokuSpace>("Single Constraint: Negative Diagonal Even", ConstraintType::NEGATIVE_DIAGONAL_EVEN);
  }

  TEST_CASE("Single Constraint: Negative Diagonal Odd") {
    check.operator()<sudokuSpace>("Single Constraint: Negative Diagonal Odd", ConstraintType::NEGATIVE_DIAGONAL_ODD);
  }

  TEST_CASE("Single Constraint: Anti King") {
    check.operator()<sudokuSpace>("Single Constraint: Anti King", ConstraintType::KING_PATTERN);
  }

  TEST_CASE("Single Constraint: Anti King Torus") {
    check.operator()<sudokuSpace>("Single Constraint: Anti King Torus", ConstraintType::KING_TORUS_PATTERN);
  }

  TEST_CASE("Single Constraint: Anti Knight") {
    check.operator()<sudokuSpace>("Single Constraint: Anti Knight", ConstraintType::KNIGHT_PATTERN);
  }

  TEST_CASE("Single Constraint: Anti Knight Torus") {
    check.operator()<sudokuSpace>("Single Constraint: Anti Knight Torus", ConstraintType::KNIGHT_TORUS_PATTERN);
  }

  TEST_CASE("Single Constraint: Disjoint Boxes") {
    check.operator()<sudokuSpace>("Single Constraint: Disjoint Boxes", ConstraintType::DISJOINT_BOXES);
  }

  TEST_CASE("Single Constraint: Asterisk") {
    check.operator()<sudokuSpace>("Single Constraint: Asterisk", ConstraintType::ASTERISK);
  }
}
