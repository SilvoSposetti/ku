#include "Puzzle.hpp"

#include <doctest.h>

TEST_CASE("Sudoku Samples") {
  constexpr auto sudokuConstraints = ConstraintType::SUDOKU_CELL | ConstraintType::SUDOKU_ROW |
                                     ConstraintType::SUDOKU_COLUMN | ConstraintType::SUDOKU_BOX;
  constexpr auto sudokuSpace = PuzzleSpace{9, 9, 9};

  SUBCASE("Large search tree") {
    constexpr auto input = Grid<sudokuSpace>{{
        {1, 2, 0, 3, 0, 0, 4, 0, 0},
        {5, 0, 0, 4, 0, 0, 1, 0, 0},
        {0, 0, 0, 0, 2, 0, 0, 6, 0},
        {7, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 7, 0, 0, 0, 3, 1},
        {0, 0, 0, 0, 5, 4, 7, 0, 0},
        {4, 0, 0, 5, 0, 0, 3, 0, 0},
        {0, 8, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 9, 0, 4, 0, 0, 0, 0},
    }};

    constexpr auto solution = Grid<sudokuSpace>{{
        {1, 2, 8, 3, 6, 5, 4, 7, 9},
        {5, 9, 6, 4, 7, 8, 1, 2, 3},
        {3, 4, 7, 9, 2, 1, 8, 6, 5},
        {7, 1, 5, 6, 3, 9, 2, 4, 8},
        {9, 6, 4, 7, 8, 2, 5, 3, 1},
        {8, 3, 2, 1, 5, 4, 7, 9, 6},
        {4, 7, 1, 5, 9, 6, 3, 8, 2},
        {6, 8, 3, 2, 1, 7, 9, 5, 4},
        {2, 5, 9, 8, 4, 3, 6, 1, 7},
    }};

    const auto puzzle = Puzzle<sudokuSpace>("LargeSearchTree", input, sudokuConstraints, 0);
    CHECK_EQ(puzzle.solution, solution);
  }

  SUBCASE("Digits of pi with 32 clues (1)") {
    // Note: The 33rd digit of pi is a 0
    constexpr auto input = Grid<sudokuSpace>{{
        {0, 0, 3, 0, 1, 0, 0, 0, 0},
        {4, 1, 5, 0, 0, 0, 0, 9, 0},
        {2, 0, 6, 5, 0, 0, 3, 0, 0},
        {5, 0, 0, 0, 8, 0, 0, 0, 9},
        {0, 7, 0, 9, 0, 0, 0, 3, 2},
        {0, 3, 8, 0, 0, 4, 0, 6, 0},
        {0, 0, 0, 2, 6, 0, 4, 0, 3},
        {0, 0, 0, 3, 0, 0, 0, 0, 8},
        {3, 2, 0, 0, 0, 7, 9, 5, 0},
    }};

    constexpr auto solution = Grid<sudokuSpace>{{
        {7, 9, 3, 4, 1, 2, 6, 8, 5},
        {4, 1, 5, 6, 3, 8, 2, 9, 7},
        {2, 8, 6, 5, 7, 9, 3, 1, 4},
        {5, 6, 2, 1, 8, 3, 7, 4, 9},
        {1, 7, 4, 9, 5, 6, 8, 3, 2},
        {9, 3, 8, 7, 2, 4, 5, 6, 1},
        {8, 5, 9, 2, 6, 1, 4, 7, 3},
        {6, 4, 7, 3, 9, 5, 1, 2, 8},
        {3, 2, 1, 8, 4, 7, 9, 5, 6},
    }};
    const auto puzzle = Puzzle<sudokuSpace>("DigitsOfPi32Clues1", input, sudokuConstraints, 0);
    CHECK_EQ(puzzle.solution, solution);
  }

  SUBCASE("Digits of pi with 32 clues (2)") {
    // Note: The 33rd digit of pi is a 0
    constexpr auto input = Grid<sudokuSpace>{{
        {0, 0, 0, 0, 0, 3, 0, 1, 4},
        {0, 0, 0, 0, 0, 1, 5, 9, 2},
        {0, 0, 0, 0, 6, 5, 0, 0, 3},
        {0, 0, 0, 0, 5, 8, 9, 0, 7},
        {0, 0, 9, 0, 0, 0, 3, 2, 0},
        {0, 3, 8, 4, 0, 0, 0, 0, 6},
        {2, 6, 0, 0, 0, 4, 0, 3, 0},
        {0, 0, 0, 0, 3, 0, 0, 8, 0},
        {0, 0, 3, 2, 7, 9, 0, 0, 5},
    }};

    constexpr auto solution = Grid<sudokuSpace>{{
        {5, 9, 7, 8, 2, 3, 6, 1, 4},
        {3, 8, 6, 7, 4, 1, 5, 9, 2},
        {1, 4, 2, 9, 6, 5, 8, 7, 3},
        {6, 2, 1, 3, 5, 8, 9, 4, 7},
        {4, 5, 9, 6, 1, 7, 3, 2, 8},
        {7, 3, 8, 4, 9, 2, 1, 5, 6},
        {2, 6, 5, 1, 8, 4, 7, 3, 9},
        {9, 7, 4, 5, 3, 6, 2, 8, 1},
        {8, 1, 3, 2, 7, 9, 4, 6, 5},
    }};

    const auto puzzle = Puzzle<sudokuSpace>("DigitsOfPi32Clues2", input, sudokuConstraints, 0);
    CHECK_EQ(puzzle.solution, solution);
  }

  SUBCASE("Digits of pi with 17 clues") {
    // 17 clues is the minimum allowed for a classic sudoku to may still have a unique solution
    constexpr auto input = Grid<sudokuSpace>{{
        {0, 0, 0, 0, 0, 0, 3, 0, 0},
        {1, 0, 0, 4, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0, 5},
        {9, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 2, 6, 0, 0},
        {0, 0, 0, 0, 5, 3, 0, 0, 0},
        {0, 5, 0, 8, 0, 0, 0, 0, 0},
        {0, 0, 0, 9, 0, 0, 0, 7, 0},
        {0, 8, 3, 0, 0, 0, 0, 4, 0},
    }};

    constexpr auto solution = Grid<sudokuSpace>{{
        {5, 9, 7, 2, 1, 8, 3, 6, 4},
        {1, 3, 2, 4, 6, 5, 8, 9, 7},
        {8, 6, 4, 3, 7, 9, 1, 2, 5},
        {9, 1, 5, 6, 8, 4, 7, 3, 2},
        {3, 4, 8, 7, 9, 2, 6, 5, 1},
        {2, 7, 6, 1, 5, 3, 4, 8, 9},
        {6, 5, 9, 8, 4, 7, 2, 1, 3},
        {4, 2, 1, 9, 3, 6, 5, 7, 8},
        {7, 8, 3, 5, 2, 1, 9, 4, 6},
    }};

    const auto puzzle = Puzzle<sudokuSpace>("DigitsOfPi17Clues", input, sudokuConstraints, 0);
    CHECK_EQ(puzzle.solution, solution);
  }

  SUBCASE("Solvable with only naked singles") {
    constexpr auto input = Grid<sudokuSpace>{{
        {0, 0, 3, 0, 1, 0, 0, 0, 0},
        {4, 1, 5, 0, 0, 0, 0, 9, 0},
        {2, 0, 6, 5, 0, 0, 3, 0, 0},
        {5, 0, 0, 0, 8, 0, 0, 0, 9},
        {0, 7, 0, 9, 0, 0, 0, 3, 2},
        {0, 3, 8, 0, 0, 4, 0, 6, 0},
        {0, 0, 0, 2, 6, 0, 4, 0, 3},
        {0, 0, 0, 3, 0, 0, 0, 0, 8},
        {3, 2, 0, 0, 0, 7, 9, 5, 0},
    }};

    constexpr auto solution = Grid<sudokuSpace>{{
        {7, 9, 3, 4, 1, 2, 6, 8, 5},
        {4, 1, 5, 6, 3, 8, 2, 9, 7},
        {2, 8, 6, 5, 7, 9, 3, 1, 4},
        {5, 6, 2, 1, 8, 3, 7, 4, 9},
        {1, 7, 4, 9, 5, 6, 8, 3, 2},
        {9, 3, 8, 7, 2, 4, 5, 6, 1},
        {8, 5, 9, 2, 6, 1, 4, 7, 3},
        {6, 4, 7, 3, 9, 5, 1, 2, 8},
        {3, 2, 1, 8, 4, 7, 9, 5, 6},
    }};

    const auto puzzle = Puzzle<sudokuSpace>("SolvableWithOnlyNakedSingles", input, sudokuConstraints, 0);
    CHECK_EQ(puzzle.solution, solution);
  }

  // Digits of pi, 16 clues: There's two solutions
  // std::vector<std::vector<int32_t>> digits = {
  //     {0, 3, 0, 0, 1, 0, 0, 0, 0},
  //     {0, 0, 0, 4, 0, 0, 1, 0, 0},
  //     {0, 5, 0, 0, 0, 0, 0, 9, 0},
  //     {2, 0, 0, 0, 0, 0, 6, 0, 4},
  //     {0, 0, 0, 0, 3, 5, 0, 0, 0},
  //     {1, 0, 0, 0, 0, 0, 0, 0, 0},
  //     {4, 0, 0, 6, 0, 0, 0, 0, 0},
  //     {0, 0, 0, 0, 0, 0, 0, 5, 0},
  //     {0, 9, 0, 0, 0, 0, 0, 0, 0},
  // };
}
