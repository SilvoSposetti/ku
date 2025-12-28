#include "KuTestArguments.hpp"
#include "constraints/ConstraintType.hpp"
#include "puzzles/Puzzle.hpp"

#include <doctest.h>

TEST_SUITE("Classic Sudoku Base") {
  const auto sudokuConstraints = ConstraintType::SUDOKU_CELL | ConstraintType::SUDOKU_COLUMN |
                                 ConstraintType::SUDOKU_ROW | ConstraintType::SUDOKU_BOX;
  constexpr auto sudokuSpace = PuzzleSpace{9, 9, 9};
  constexpr auto emptyGrid = Grid<sudokuSpace>{};

  const auto check = []<PuzzleSpace space>(const std::string& name, ConstraintType constraints) {
    const auto puzzle = Puzzle<sudokuSpace>(name, emptyGrid, constraints, KuTestArguments::seed);
    CHECK_NE(puzzle.solution, emptyGrid);
  };

  TEST_CASE("Classic Sudoku") {
    check.operator()<sudokuSpace>("Classic Sudoku", sudokuConstraints);
  }

  TEST_CASE("Classic Sudoku Base: Positive Diagonal") {
    check.operator()<sudokuSpace>("Classic Sudoku Base: Positive Diagonal",
                                  sudokuConstraints | ConstraintType::POSITIVE_DIAGONAL);
  }

  TEST_CASE("Classic Sudoku Base: Positive Diagonal Even") {
    check.operator()<sudokuSpace>("Classic Sudoku Base: Positive Diagonal Even",
                                  sudokuConstraints | ConstraintType::POSITIVE_DIAGONAL_EVEN);
  }

  TEST_CASE("Classic Sudoku Base: Positive Diagonal Odd") {
    check.operator()<sudokuSpace>("Classic Sudoku Base: Positive Diagonal Odd",
                                  sudokuConstraints | ConstraintType::POSITIVE_DIAGONAL_ODD);
  }

  TEST_CASE("Classic Sudoku Base: Negative Diagonal") {
    check.operator()<sudokuSpace>("Classic Sudoku Base: Negative Diagonal",
                                  sudokuConstraints | ConstraintType::NEGATIVE_DIAGONAL);
  }

  TEST_CASE("Classic Sudoku Base: Negative Diagonal Even") {
    check.operator()<sudokuSpace>("Classic Sudoku Base: Negative Diagonal Even",
                                  sudokuConstraints | ConstraintType::NEGATIVE_DIAGONAL_EVEN);
  }

  TEST_CASE("Classic Sudoku Base: Negative Diagonal Odd") {
    check.operator()<sudokuSpace>("Classic Sudoku Base: Negative Diagonal Odd",
                                  sudokuConstraints | ConstraintType::NEGATIVE_DIAGONAL_ODD);
  }

  TEST_CASE("Classic Sudoku Base: Anti King") {
    check.operator()<sudokuSpace>("Classic Sudoku Base: Anti King", sudokuConstraints | ConstraintType::KING_PATTERN);
  }

  TEST_CASE("Classic Sudoku Base: Anti King Torus") {
    check.operator()<sudokuSpace>("Classic Sudoku Base: Anti King Torus",
                                  sudokuConstraints | ConstraintType::KING_TORUS_PATTERN);
  }

  TEST_CASE("Classic Sudoku Base: Anti Knight") {
    check.operator()<sudokuSpace>("Classic Sudoku Base: Anti Knight",
                                  sudokuConstraints | ConstraintType::KNIGHT_PATTERN);
  }

  TEST_CASE("Classic Sudoku Base: Anti Knight Torus") {
    check.operator()<sudokuSpace>("Classic Sudoku Base: Anti Knight Torus",
                                  sudokuConstraints | ConstraintType::KNIGHT_TORUS_PATTERN);
  }

  TEST_CASE("Classic Sudoku Base: Disjoint Boxes") {
    check.operator()<sudokuSpace>("Classic Sudoku Base: Disjoint Boxes",
                                  sudokuConstraints | ConstraintType::DISJOINT_BOXES);
  }

  TEST_CASE("Classic Sudoku Base: Asterisk") {
    check.operator()<sudokuSpace>("Classic Sudoku Base: Asterisk", sudokuConstraints | ConstraintType::ASTERISK);
  }
}
