#include "puzzles/Puzzle.hpp"

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

  // SUBCASE("Grid as text") {
  //   SUBCASE("1x1") {
  //     const auto puzzle = Puzzle<{1, 1, 9}>("1x1", {}, ConstraintType::NONE, {});
  //     const auto expected = std::vector<std::string>{
  //         "┏━━━┓",
  //         "┃ ◌ ┃",
  //         "┗━━━┛",
  //     };
  //     CHECK_EQ(puzzle.gridAsText(), expected);
  //   }
  //   SUBCASE("1x1 with given") {
  //     const auto puzzle = Puzzle<{1, 1, 9}>("1x1", {{0, 0, 5}}, ConstraintType::NONE, {});
  //     const auto expected = std::vector<std::string>{
  //         "┏━━━┓",
  //         "┃ 5 ┃",
  //         "┗━━━┛",
  //     };
  //     CHECK_EQ(puzzle.gridAsText(), expected);
  //   }
  //   SUBCASE("1x2") {
  //     const auto puzzle = Puzzle<{1, 2, 9}>("1x2", {}, ConstraintType::NONE, {});
  //     const auto expected = std::vector<std::string>{
  //         "┏━━━━━┓",
  //         "┃ ◌ ◌ ┃",
  //         "┗━━━━━┛",
  //     };
  //     CHECK_EQ(puzzle.gridAsText(), expected);
  //   }
  //   SUBCASE("1x2 with givens") {
  //     const auto puzzle = Puzzle<{1, 2, 9}>("1x2", {{0, 0, 1}, {0, 1, 2}}, ConstraintType::NONE, {});
  //     const auto expected = std::vector<std::string>{
  //         "┏━━━━━┓",
  //         "┃ 1 2 ┃",
  //         "┗━━━━━┛",
  //     };
  //     CHECK_EQ(puzzle.gridAsText(), expected);
  //   }
  //   SUBCASE("2x1") {
  //     const auto puzzle = Puzzle<{2, 1, 9}>("2x1", {}, ConstraintType::NONE, {});
  //     const auto expected = std::vector<std::string>{
  //         "┏━━━┓",
  //         "┃ ◌ ┃",
  //         "┃ ◌ ┃",
  //         "┗━━━┛",
  //     };
  //     CHECK_EQ(puzzle.gridAsText(), expected);
  //   }
  //   SUBCASE("2x1 with given") {
  //     const auto puzzle = Puzzle<{2, 1, 9}>("2x1", {{1, 0, 8}}, ConstraintType::NONE, {});
  //     const auto expected = std::vector<std::string>{
  //         "┏━━━┓",
  //         "┃ ◌ ┃",
  //         "┃ 8 ┃",
  //         "┗━━━┛",
  //     };
  //     CHECK_EQ(puzzle.gridAsText(), expected);
  //   }
  //   SUBCASE("5x7") {
  //     const auto puzzle = Puzzle<{5, 7, 9}>("5x7", {}, ConstraintType::NONE, {});
  //     const auto expected = std::vector<std::string>{
  //         "┏━━━━━━━━━━━━━━━┓",
  //         "┃ ◌ ◌ ◌ ◌ ◌ ◌ ◌ ┃",
  //         "┃ ◌ ◌ ◌ ◌ ◌ ◌ ◌ ┃",
  //         "┃ ◌ ◌ ◌ ◌ ◌ ◌ ◌ ┃",
  //         "┃ ◌ ◌ ◌ ◌ ◌ ◌ ◌ ┃",
  //         "┃ ◌ ◌ ◌ ◌ ◌ ◌ ◌ ┃",
  //         "┗━━━━━━━━━━━━━━━┛",
  //     };
  //     CHECK_EQ(puzzle.gridAsText(), expected);
  //   }
  //   SUBCASE("5x7 with givens") {
  //     const auto puzzle = Puzzle<{5, 7, 9}>(
  //         "5x7", {{1, 1, 1}, {1, 5, 4}, {2, 2, 2}, {3, 4, 3}, {4, 1, 5}, {4, 6, 6}}, ConstraintType::NONE, {});
  //     const auto expected = std::vector<std::string>{
  //         "┏━━━━━━━━━━━━━━━┓",
  //         "┃ ◌ ◌ ◌ ◌ ◌ ◌ ◌ ┃",
  //         "┃ ◌ 1 ◌ ◌ ◌ 4 ◌ ┃",
  //         "┃ ◌ ◌ 2 ◌ ◌ ◌ ◌ ┃",
  //         "┃ ◌ ◌ ◌ ◌ 3 ◌ ◌ ┃",
  //         "┃ ◌ 5 ◌ ◌ ◌ ◌ 6 ┃",
  //         "┗━━━━━━━━━━━━━━━┛",
  //     };
  //     CHECK_EQ(puzzle.gridAsText(), expected);
  //   }
  // }
}