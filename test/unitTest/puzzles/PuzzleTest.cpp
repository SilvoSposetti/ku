#include "puzzles/Puzzle.hpp"

#include <doctest.h>

/** Using here the classic Sudoku class as replacement for the Puzzle class
 */
TEST_CASE("Puzzle") {
  const auto seed = 0;

  SUBCASE("Construction") {
    const auto name = std::string("Test");
    const auto puzzle = Puzzle<1, 1, 1>(name, {}, ConstraintType::NONE, seed);
    CHECK(puzzle.seed == seed);
    CHECK(puzzle.name == name);
  }

  SUBCASE("Grid Initialization With No Clues") {
    const auto puzzle = Puzzle<9, 9, 9>("Test", {}, ConstraintType::NONE, {});
    for (const auto& row : puzzle.grid) {
      CHECK(std::ranges::all_of(row, [&](const auto& element) { return element == Digits::invalidDigit; }));
    }
  }

  SUBCASE("Grid Initialization With Clues") {
    const auto clues = std::vector{
        Cell(0, 0, 1),
        Cell(0, 1, 2),
        Cell(0, 2, 3),
        Cell(1, 0, 4),
    };
    const auto puzzle = Puzzle<9, 9, 9>("Test", clues, ConstraintType::NONE, {});
    for (const auto& cell : clues) {
      CHECK(puzzle.grid[cell.rowIndex][cell.columnIndex] == cell.digit);
    }
  }

  SUBCASE("Constraint construction") {
    const auto constraints = ConstraintType::SUDOKU_CELL | ConstraintType::SUDOKU_ROW | ConstraintType::SUDOKU_COLUMN |
                             ConstraintType::SUDOKU_BOX;
    const auto puzzle = Puzzle<9, 9, 9>("Test", {}, constraints, {});
    for (const auto& constraint : puzzle.constraints) {
      CHECK(static_cast<bool>(constraints & constraint->getType()));
    }
  }

  SUBCASE("Clues handling") {
    SUBCASE("No clues") {
      const auto puzzle = Puzzle<9, 9, 9>("Test", {}, ConstraintType::NONE, {});
      std::vector<Cell> expected = {};
      CHECK_EQ(puzzle.givenCells, expected);
    }
    SUBCASE("Valid clues") {
      const auto puzzle = Puzzle<9, 9, 9>("Test", {{0, 1, 2}, {2, 3, 3}}, ConstraintType::NONE, {});
      std::vector<Cell> expected = {{0, 1, 2}, {2, 3, 3}};
      CHECK_EQ(puzzle.givenCells, expected);
    }
    SUBCASE("Invalid row") {
      const auto puzzle = Puzzle<9, 9, 9>("Test", {{15, 1, 2}}, ConstraintType::NONE, {});
      std::vector<Cell> expected = {};
      CHECK_EQ(puzzle.givenCells, expected);
    }
    SUBCASE("Invalid column") {
      const auto puzzle = Puzzle<9, 9, 9>("Test", {{0, 15, 2}}, ConstraintType::NONE, {});
      std::vector<Cell> expected = {};
      CHECK_EQ(puzzle.givenCells, expected);
    }
    SUBCASE("Invalid digit") {
      const auto puzzle = Puzzle<9, 9, 9>("Test", {{0, 1, 15}}, ConstraintType::NONE, {});
      std::vector<Cell> expected = {};
      CHECK_EQ(puzzle.givenCells, expected);
    }
    SUBCASE("Some clues invalid") {
      const auto puzzle =
          Puzzle<9, 9, 9>("Test", {{0, 1, 2}, {5, 5, 5}, {1, 3, 10}, {0, 1, 2}, {12, 0, 1}}, ConstraintType::NONE, {});
      std::vector<Cell> expected = {{0, 1, 2}, {5, 5, 5}, {0, 1, 2}};
      CHECK_EQ(puzzle.givenCells, expected);
    }
  }

  SUBCASE("Grid as text") {
    SUBCASE("1x1") {
      const auto puzzle = Puzzle<1, 1, 9>("1x1", {}, ConstraintType::NONE, {});
      const auto expected = std::vector<std::string>{
          "┏━━━┓",
          "┃ ◌ ┃",
          "┗━━━┛",
      };
      CHECK_EQ(puzzle.gridAsText(), expected);
    }
    SUBCASE("1x1 with given") {
      const auto puzzle = Puzzle<1, 1, 9>("1x1", {{0, 0, 5}}, ConstraintType::NONE, {});
      const auto expected = std::vector<std::string>{
          "┏━━━┓",
          "┃ 5 ┃",
          "┗━━━┛",
      };
      CHECK_EQ(puzzle.gridAsText(), expected);
    }
    SUBCASE("1x2") {
      const auto puzzle = Puzzle<1, 2, 9>("1x2", {}, ConstraintType::NONE, {});
      const auto expected = std::vector<std::string>{
          "┏━━━━━┓",
          "┃ ◌ ◌ ┃",
          "┗━━━━━┛",
      };
      CHECK_EQ(puzzle.gridAsText(), expected);
    }
    SUBCASE("1x2 with givens") {
      const auto puzzle = Puzzle<1, 2, 9>("1x2", {{0, 0, 1}, {0, 1, 2}}, ConstraintType::NONE, {});
      const auto expected = std::vector<std::string>{
          "┏━━━━━┓",
          "┃ 1 2 ┃",
          "┗━━━━━┛",
      };
      CHECK_EQ(puzzle.gridAsText(), expected);
    }
    SUBCASE("2x1") {
      const auto puzzle = Puzzle<2, 1, 9>("2x1", {}, ConstraintType::NONE, {});
      const auto expected = std::vector<std::string>{
          "┏━━━┓",
          "┃ ◌ ┃",
          "┃ ◌ ┃",
          "┗━━━┛",
      };
      CHECK_EQ(puzzle.gridAsText(), expected);
    }
    SUBCASE("2x1 with given") {
      const auto puzzle = Puzzle<2, 1, 9>("2x1", {{1, 0, 8}}, ConstraintType::NONE, {});
      const auto expected = std::vector<std::string>{
          "┏━━━┓",
          "┃ ◌ ┃",
          "┃ 8 ┃",
          "┗━━━┛",
      };
      CHECK_EQ(puzzle.gridAsText(), expected);
    }
    SUBCASE("5x7") {
      const auto puzzle = Puzzle<5, 7, 9>("5x7", {}, ConstraintType::NONE, {});
      const auto expected = std::vector<std::string>{
          "┏━━━━━━━━━━━━━━━┓",
          "┃ ◌ ◌ ◌ ◌ ◌ ◌ ◌ ┃",
          "┃ ◌ ◌ ◌ ◌ ◌ ◌ ◌ ┃",
          "┃ ◌ ◌ ◌ ◌ ◌ ◌ ◌ ┃",
          "┃ ◌ ◌ ◌ ◌ ◌ ◌ ◌ ┃",
          "┃ ◌ ◌ ◌ ◌ ◌ ◌ ◌ ┃",
          "┗━━━━━━━━━━━━━━━┛",
      };
      CHECK_EQ(puzzle.gridAsText(), expected);
    }
    SUBCASE("5x7 with givens") {
      const auto puzzle = Puzzle<5, 7, 9>(
          "5x7", {{1, 1, 1}, {1, 5, 4}, {2, 2, 2}, {3, 4, 3}, {4, 1, 5}, {4, 6, 6}}, ConstraintType::NONE, {});
      const auto expected = std::vector<std::string>{
          "┏━━━━━━━━━━━━━━━┓",
          "┃ ◌ ◌ ◌ ◌ ◌ ◌ ◌ ┃",
          "┃ ◌ 1 ◌ ◌ ◌ 4 ◌ ┃",
          "┃ ◌ ◌ 2 ◌ ◌ ◌ ◌ ┃",
          "┃ ◌ ◌ ◌ ◌ 3 ◌ ◌ ┃",
          "┃ ◌ 5 ◌ ◌ ◌ ◌ 6 ┃",
          "┗━━━━━━━━━━━━━━━┛",
      };
      CHECK_EQ(puzzle.gridAsText(), expected);
    }
  }
}