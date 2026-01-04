#include "Puzzle.hpp"

#include <doctest.h>

/** Using here the classic Sudoku class as replacement for the Puzzle class
 */
TEST_CASE("Puzzle") {

  SUBCASE("Construction") {
    const auto seed = 0;
    const auto name = std::string("Test");
    const auto puzzle = Puzzle<{1, 1, 1}>(name, {}, ConstraintType::NONE, seed);
    CHECK_EQ(puzzle.seed, seed);
    CHECK_EQ(puzzle.name, name);
  }

  SUBCASE("Grid Initialization With No Clues") {
    const auto puzzle = Puzzle<{9, 9, 9}>("Test", {}, ConstraintType::SUDOKU_ROW, {});
    for (const auto& row : puzzle.startingGrid) {
      CHECK(std::ranges::all_of(row, [&](const auto& element) { return element == Digits::invalidDigit; }));
    }
  }

  SUBCASE("Grid Initialization With Clues") {
    constexpr auto puzzleSpace = PuzzleSpace{3, 3, 3};
    const auto startingGrid = Grid<puzzleSpace>{{
        {1, 0, 1},
        {3, 0, 0},
        {0, 2, 0},
    }};
    const auto puzzle = Puzzle<puzzleSpace>("Test", startingGrid, ConstraintType::NONE, {});
    CHECK_EQ(puzzle.startingGrid, startingGrid);
  }

  SUBCASE("Constraint construction") {
    constexpr auto sudokuConstraints = ConstraintType::SUDOKU_CELL | ConstraintType::SUDOKU_ROW |
                                       ConstraintType::SUDOKU_COLUMN | ConstraintType::SUDOKU_BOX;
    const auto puzzle = Puzzle<{9, 9, 9}>("Test", {}, sudokuConstraints, {});
    for (const auto& constraint : puzzle.constraints) {
      CHECK(static_cast<bool>(sudokuConstraints & constraint->getType()));
    }
  }

  SUBCASE("Possibilities") {
    SUBCASE("No clues") {
      const auto puzzle = Puzzle<{9, 9, 9}>("Test", {}, ConstraintType::NONE, {});
      const auto allPoissibilitiesArray = puzzle.allPossibilities();
      const auto allPossibilities = std::vector<Cell>(allPoissibilitiesArray.begin(), allPoissibilitiesArray.end());
      const auto possibilities = puzzle.possibilities;
      CHECK_EQ(allPossibilities, possibilities);
    }

    SUBCASE("Valid clues") {
      constexpr auto puzzleSpace = PuzzleSpace{2, 2, 3};
      const auto startingGrid = Grid<puzzleSpace>{{
          {0, 2},
          {3, 0},
      }};
      const auto puzzle = Puzzle<puzzleSpace>("Test", startingGrid, ConstraintType::NONE, {});
      const auto allPoissibilitiesArray = puzzle.allPossibilities();
      const auto allPossibilities = std::vector<Cell>(allPoissibilitiesArray.begin(), allPoissibilitiesArray.end());
      const auto possibilities = puzzle.possibilities;
      CHECK_EQ(allPossibilities.size(), 12);
      CHECK_EQ(possibilities.size(), 8); // Two given digits reduce the possibilities by ((digitsCount - 1) = 2) twice.
    }
  }

  SUBCASE("Randomness") {
    constexpr ConstraintType sudokuConstraints = ConstraintType::SUDOKU_CELL | ConstraintType::SUDOKU_ROW |
                                                 ConstraintType::SUDOKU_COLUMN | ConstraintType::SUDOKU_BOX;
    constexpr auto sudokuSpace = PuzzleSpace{9, 9, 9};

    SUBCASE("Random Seed") {
      const auto sudoku1 = Puzzle<sudokuSpace>("Sudoku1", {}, sudokuConstraints, {});
      const auto sudoku2 = Puzzle<sudokuSpace>("Sudoku2", {}, sudokuConstraints, {});
      CHECK_NE(sudoku1.solution, sudoku2.solution);
    }

    SUBCASE("Different Seed") {
      const auto sudoku1 = Puzzle<sudokuSpace>("Sudoku1", {}, sudokuConstraints, 0);
      const auto sudoku2 = Puzzle<sudokuSpace>("Sudoku2", {}, sudokuConstraints, 1);
      CHECK_NE(sudoku1.solution, sudoku2.solution);
    }

    SUBCASE("Same Seed") {
      const auto sudoku1 = Puzzle<sudokuSpace>("Sudoku1", {}, sudokuConstraints, 0);
      const auto sudoku2 = Puzzle<sudokuSpace>("Sudoku2", {}, sudokuConstraints, 0);
      CHECK_EQ(sudoku1.solution, sudoku2.solution);
    }
  }
}
