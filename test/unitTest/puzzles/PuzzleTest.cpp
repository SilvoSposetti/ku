#include "puzzles/Puzzle.hpp"

#include <algorithm>
#include <doctest.h>
#include <set>

TEST_SUITE("Puzzle") {
  TEST_CASE("Template instantiation") {
    constexpr uint8_t rows = 8;
    constexpr uint8_t columns = 5;
    constexpr uint8_t digitsCount = 9;

    constexpr auto puzzle = Puzzle<rows, columns, digitsCount>();

    // rows and columns become available as members
    CHECK_EQ(puzzle.rows, rows);
    CHECK_EQ(puzzle.columns, columns);

    // digitsCount reflects a specific set of digits that needs to be constructed.
    CHECK_EQ(puzzle.digits.size(), digitsCount);
    CHECK(std::ranges::is_sorted(puzzle.digits));
    CHECK_EQ(puzzle.digits.front(), 1);
    CHECK_EQ(puzzle.digits.back(), digitsCount);

    // All digits are different
    auto digitsSet = std::set<Digit>(puzzle.digits.begin(), puzzle.digits.end());
  }

  TEST_CASE_TEMPLATE_DEFINE("Compile-time construction", T, test_id) {

    constexpr T puzzle;

    SUBCASE("Rows and row indices") {
      // Row indices are enough and sorted
      CHECK_EQ(puzzle.rowIndices.size(), puzzle.rows);
      CHECK(std::ranges::is_sorted(puzzle.rowIndices));
      if (puzzle.rows > 0) {
        CHECK_EQ(puzzle.rowIndices.front(), 0);
        CHECK_EQ(puzzle.rowIndices.back(), puzzle.rows - 1);
      }
    }

    SUBCASE("Columns and column indices") {
      // Column indices are enough and sorted
      CHECK_EQ(puzzle.columns, puzzle.columnIndices.size());
      CHECK(std::ranges::is_sorted(puzzle.columnIndices));
      if (puzzle.columns > 0) {
        CHECK_EQ(puzzle.columnIndices.front(), 0);
        CHECK_EQ(puzzle.columnIndices.back(), puzzle.columns - 1);
      }
    }

    SUBCASE("Digits") {
      // Digits are sorted
      CHECK(std::ranges::is_sorted(puzzle.digits));

      if (puzzle.digits.size() > 0) {
        CHECK_EQ(puzzle.digits.front(), 1);
        CHECK_EQ(puzzle.digits.back(), puzzle.digits.size());
      }
    }

    SUBCASE("Grid") {
      // The correct amount of rows appear in the grid
      CHECK_EQ(puzzle.grid.size(), puzzle.rows);
      for (const auto& row : puzzle.grid) {
        // All rows have the correct size
        CHECK_EQ(row.size(), puzzle.columns);
        // All values in the rows are initialized with the invalid digit
        CHECK(std::ranges::all_of(row, [&](const auto& digit) { return digit == Digits::invalidDigit; }));
      }
    }

    SUBCASE("Possibilites") {
      // One possibility for each row, column, and digit
      constexpr auto possibilitiesCount = puzzle.rows * puzzle.columns * puzzle.digits.size();
      CHECK_EQ(puzzle.possibilities.size(), possibilitiesCount);

      // All possibilities should appear in the correct order:
      std::size_t i = 0;
      for (const auto& rowIndex : puzzle.rowIndices) {
        for (const auto& columnIndex : puzzle.columnIndices) {
          for (const auto& digit : puzzle.digits) {
            const auto& cell = puzzle.possibilities[i];
            CHECK_EQ(cell.rowIndex, rowIndex);
            CHECK_EQ(cell.columnIndex, columnIndex);
            CHECK_EQ(cell.digit, digit);
            i++;
          }
        }
      }
    }
  }

  // Cases with zero
  TEST_CASE_TEMPLATE_INVOKE(test_id, Puzzle<0, 0, 0>);
  TEST_CASE_TEMPLATE_INVOKE(test_id, Puzzle<1, 1, 0>);
  TEST_CASE_TEMPLATE_INVOKE(test_id, Puzzle<1, 0, 1>);
  TEST_CASE_TEMPLATE_INVOKE(test_id, Puzzle<0, 1, 1>);
  TEST_CASE_TEMPLATE_INVOKE(test_id, Puzzle<1, 0, 0>);
  TEST_CASE_TEMPLATE_INVOKE(test_id, Puzzle<0, 1, 0>);
  TEST_CASE_TEMPLATE_INVOKE(test_id, Puzzle<0, 0, 1>);
  // More "regular" cases
  TEST_CASE_TEMPLATE_INVOKE(test_id, Puzzle<1, 2, 5>);
  TEST_CASE_TEMPLATE_INVOKE(test_id, Puzzle<3, 3, 3>);
  TEST_CASE_TEMPLATE_INVOKE(test_id, Puzzle<6, 6, 2>);
  TEST_CASE_TEMPLATE_INVOKE(test_id, Puzzle<9, 9, 9>);
  TEST_CASE_TEMPLATE_INVOKE(test_id, Puzzle<7, 12, 5>);
  TEST_CASE_TEMPLATE_INVOKE(test_id, Puzzle<13, 12, 11>);
  TEST_CASE_TEMPLATE_INVOKE(test_id, Puzzle<15, 7, 21>);
  TEST_CASE_TEMPLATE_INVOKE(test_id, Puzzle<1, 100, 3>);
}