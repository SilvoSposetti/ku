#include "puzzles/PuzzleIntrinsics.hpp"

#include <algorithm>
#include <doctest.h>
#include <set>

TEST_SUITE("PuzzleIntrinsiscs") {
  TEST_CASE("Template instantiation") {
    constexpr uint8_t rows = 8;
    constexpr uint8_t columns = 5;
    constexpr uint8_t digitsCount = 9;

    constexpr auto puzzle = PuzzleIntrinsics<{rows, columns, digitsCount}>();

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

  TEST_CASE("Cell Validiy") {
    constexpr uint8_t rows = 3;
    constexpr uint8_t columns = 4;
    constexpr uint8_t digitsCount = 5;

    constexpr auto puzzle = PuzzleIntrinsics<{rows, columns, digitsCount}>();

    constexpr auto validCells = std::array{
        Cell(0, 0, 1),
        Cell(1, 1, 2),
        Cell(2, 2, 3),
        Cell(2, 3, 4),
        Cell(2, 3, 5),
    };

    for (const auto& validCell : validCells) {
      CHECK(puzzle.isCellValid(validCell));
    }

    constexpr auto invalidCells = std::array{
        Cell(3, 0, 1), // invalid row index
        Cell(3, 1, 2), // invalid row index
        Cell(0, 4, 3), // invalid column index
        Cell(1, 4, 4), // invalid column index
        Cell(0, 2, 6), // invalid digit
        Cell(1, 3, 6), // invalid digit
    };

    for (const auto& invalidCell : invalidCells) {
      CHECK_FALSE(puzzle.isCellValid(invalidCell));
    }
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
        for (const auto& rowIndex : puzzle.rowIndices) {
          CHECK(puzzle.isRowIndexValid(rowIndex));
        }
      }
      CHECK_FALSE(puzzle.isRowIndexValid(puzzle.rows + 1));
    }

    SUBCASE("Columns and column indices") {
      // Column indices are enough and sorted
      CHECK_EQ(puzzle.columns, puzzle.columnIndices.size());
      CHECK(std::ranges::is_sorted(puzzle.columnIndices));
      if (puzzle.columns > 0) {
        CHECK_EQ(puzzle.columnIndices.front(), 0);
        CHECK_EQ(puzzle.columnIndices.back(), puzzle.columns - 1);
        for (const auto& columnIndex : puzzle.columnIndices) {
          CHECK(puzzle.isColumnIndexValid(columnIndex));
        }
      }
      CHECK_FALSE(puzzle.isColumnIndexValid(puzzle.columns + 1));
    }

    SUBCASE("Digits") {
      // Digits are sorted
      CHECK(std::ranges::is_sorted(puzzle.digits));

      if (puzzle.digits.size() > 0) {
        CHECK_EQ(puzzle.digits.front(), 1);
        CHECK_EQ(puzzle.digits.back(), puzzle.digits.size());
        for (const auto& digit : puzzle.digits) {
          CHECK(puzzle.isDigitValid(digit));
        }
        CHECK_FALSE(puzzle.isDigitValid(puzzle.digits.back() + 1));
      }
    }

    SUBCASE("Grid") {
      // The correct amount of rows appear in the grid
      CHECK_EQ(puzzle.emptyGrid.size(), puzzle.rows);
      for (const auto& row : puzzle.emptyGrid) {
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
  TEST_CASE_TEMPLATE_INVOKE(test_id, PuzzleIntrinsics<{0, 0, 0}>);
  TEST_CASE_TEMPLATE_INVOKE(test_id, PuzzleIntrinsics<{1, 1, 0}>);
  TEST_CASE_TEMPLATE_INVOKE(test_id, PuzzleIntrinsics<{1, 0, 1}>);
  TEST_CASE_TEMPLATE_INVOKE(test_id, PuzzleIntrinsics<{0, 1, 1}>);
  TEST_CASE_TEMPLATE_INVOKE(test_id, PuzzleIntrinsics<{1, 0, 0}>);
  TEST_CASE_TEMPLATE_INVOKE(test_id, PuzzleIntrinsics<{0, 1, 0}>);
  TEST_CASE_TEMPLATE_INVOKE(test_id, PuzzleIntrinsics<{0, 0, 1}>);

  // More "regular" cases
  TEST_CASE_TEMPLATE_INVOKE(test_id, PuzzleIntrinsics<{1, 2, 5}>);
  TEST_CASE_TEMPLATE_INVOKE(test_id, PuzzleIntrinsics<{3, 3, 3}>);
  TEST_CASE_TEMPLATE_INVOKE(test_id, PuzzleIntrinsics<{6, 6, 2}>);
  TEST_CASE_TEMPLATE_INVOKE(test_id, PuzzleIntrinsics<{9, 9, 9}>);
  TEST_CASE_TEMPLATE_INVOKE(test_id, PuzzleIntrinsics<{7, 12, 5}>);
  TEST_CASE_TEMPLATE_INVOKE(test_id, PuzzleIntrinsics<{13, 12, 11}>);
  TEST_CASE_TEMPLATE_INVOKE(test_id, PuzzleIntrinsics<{15, 7, 21}>);
  TEST_CASE_TEMPLATE_INVOKE(test_id, PuzzleIntrinsics<{1, 100, 3}>);
}