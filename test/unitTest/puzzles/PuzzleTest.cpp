#include "puzzles/Sudoku.hpp"

#include <doctest.h>

/** Using here the classic Sudoku class as replacement for the Puzzle class
 */
TEST_CASE("Puzzle") {

  SUBCASE("Construction") {
    const auto seed = 5;
    const auto name = std::string("Test");
    const auto sudoku = Sudoku(name, {}, ConstraintType::NONE, seed);
    CHECK(sudoku.seed == seed);
    CHECK(sudoku.name == name);
  }

  SUBCASE("Grid Initialization With No Clues") {
    const auto sudoku = Sudoku("Test", {}, ConstraintType::NONE, {});
    for (const auto& row : sudoku.grid) {
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
    const auto sudoku = Sudoku("Test", clues, ConstraintType::NONE, {});
    for (const auto& cell : clues) {
      CHECK(sudoku.grid[cell.rowIndex][cell.columnIndex] == cell.digit);
    }
  }

  SUBCASE("Constraint construction") {
    const auto constraints = ConstraintType::SUDOKU_CELL | ConstraintType::SUDOKU_ROW | ConstraintType::SUDOKU_COLUMN |
                             ConstraintType::SUDOKU_BOX;
    const auto sudoku = Sudoku("Test", {}, constraints, {});
    for (const auto& constraint : sudoku.constraints) {
      CHECK(static_cast<bool>(constraints & constraint->getType()));
    }
  }
}