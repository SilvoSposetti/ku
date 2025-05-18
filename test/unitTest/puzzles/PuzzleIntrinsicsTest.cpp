#include "puzzles/PuzzleIntrinsics.hpp"

#include <algorithm>
#include <doctest.h>
#include <set>

TEST_SUITE("Puzzle Intrinsiscs") {

  TEST_CASE("Template Instantiation") {
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
      CHECK(puzzle.isValidCell(validCell));
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
      CHECK_FALSE(puzzle.isValidCell(invalidCell));
    }
  }

  TEST_CASE("Is Square Puzzle") {
    SUBCASE("Invalid") {
      CHECK_FALSE(PuzzleIntrinsics<{0, 2, 0}>{}.isSquare());
      CHECK_FALSE(PuzzleIntrinsics<{1, 2, 1}>{}.isSquare());
      CHECK_FALSE(PuzzleIntrinsics<{5, 6, 2}>{}.isSquare());
    }
    SUBCASE("Valid") {
      CHECK(PuzzleIntrinsics<{0, 0, 0}>{}.isSquare());
      CHECK(PuzzleIntrinsics<{1, 1, 1}>{}.isSquare());
      CHECK(PuzzleIntrinsics<{2, 2, 2}>{}.isSquare());
      CHECK(PuzzleIntrinsics<{6, 6, 3}>{}.isSquare());
    }
  }

  TEST_CASE("Is On Positive Diagonal") {
    SUBCASE("No Rows Or Columns") {
      CHECK_FALSE(PuzzleIntrinsics<{0, 0, 5}>{}.isOnPositiveDiagonal(0, 0));
      CHECK_FALSE(PuzzleIntrinsics<{0, 0, 5}>{}.isOnPositiveDiagonal(0, 0));
      CHECK_FALSE(PuzzleIntrinsics<{0, 1, 5}>{}.isOnPositiveDiagonal(0, 0));
      CHECK_FALSE(PuzzleIntrinsics<{1, 0, 5}>{}.isOnPositiveDiagonal(0, 0));
    }
    SUBCASE("Puzzle Is Not A Square") {
      CHECK_FALSE(PuzzleIntrinsics<{8, 7, 5}>{}.isOnPositiveDiagonal(0, 0));
      CHECK_FALSE(PuzzleIntrinsics<{7, 8, 5}>{}.isOnPositiveDiagonal(0, 0));
    }
    SUBCASE("Is Outside Of Puzzle") {
      CHECK_FALSE(PuzzleIntrinsics<{5, 5, 5}>{}.isOnPositiveDiagonal(7, 7));
    }
    SUBCASE("Not On Diagonal") {
      CHECK_FALSE(PuzzleIntrinsics<{7, 7, 5}>{}.isOnPositiveDiagonal(1, 2));
      CHECK_FALSE(PuzzleIntrinsics<{7, 7, 5}>{}.isOnPositiveDiagonal(2, 0));
    }
    SUBCASE("Valid") {
      CHECK(PuzzleIntrinsics<{1, 1, 5}>{}.isOnPositiveDiagonal(0, 0));
      CHECK(PuzzleIntrinsics<{7, 7, 5}>{}.isOnPositiveDiagonal(0, 6));
      CHECK(PuzzleIntrinsics<{7, 7, 5}>{}.isOnPositiveDiagonal(6, 0));
      CHECK(PuzzleIntrinsics<{5, 5, 5}>{}.isOnPositiveDiagonal(2, 2));
      CHECK(PuzzleIntrinsics<{12, 12, 5}>{}.isOnPositiveDiagonal(4, 7));
      CHECK(PuzzleIntrinsics<{12, 12, 5}>{}.isOnPositiveDiagonal(7, 4));
    }
  }

  TEST_CASE("Is On Negative Diagonal") {
    SUBCASE("No Rows Or Columns") {
      CHECK_FALSE(PuzzleIntrinsics<{0, 0, 5}>{}.isOnNegativeDiagonal(0, 0));
      CHECK_FALSE(PuzzleIntrinsics<{0, 1, 5}>{}.isOnNegativeDiagonal(0, 0));
      CHECK_FALSE(PuzzleIntrinsics<{1, 0, 5}>{}.isOnNegativeDiagonal(0, 0));
    }
    SUBCASE("Puzzle Is Not A Square") {
      CHECK_FALSE(PuzzleIntrinsics<{8, 7, 5}>{}.isOnNegativeDiagonal(0, 0));
      CHECK_FALSE(PuzzleIntrinsics<{7, 8, 5}>{}.isOnNegativeDiagonal(0, 0));
    }
    SUBCASE("Is Outside Of Puzzle") {
      CHECK_FALSE(PuzzleIntrinsics<{5, 5, 5}>{}.isOnNegativeDiagonal(7, 7));
    }
    SUBCASE("Not On Diagonal") {
      CHECK_FALSE(PuzzleIntrinsics<{7, 7, 5}>{}.isOnNegativeDiagonal(1, 2));
      CHECK_FALSE(PuzzleIntrinsics<{7, 7, 5}>{}.isOnNegativeDiagonal(2, 0));
    }
    SUBCASE("Valid") {
      CHECK(PuzzleIntrinsics<{1, 1, 5}>{}.isOnNegativeDiagonal(0, 0));
      CHECK(PuzzleIntrinsics<{7, 7, 5}>{}.isOnNegativeDiagonal(1, 1));
      CHECK(PuzzleIntrinsics<{5, 5, 5}>{}.isOnNegativeDiagonal(0, 0));
      CHECK(PuzzleIntrinsics<{12, 12, 5}>{}.isOnNegativeDiagonal(7, 7));
    }
  }

  TEST_CASE_TEMPLATE_DEFINE("Compile-time construction", T, test_id) {

    constexpr T puzzle;

    SUBCASE("Rows And Row Indices") {
      // Row indices are enough and sorted
      CHECK_EQ(puzzle.rowIndices.size(), puzzle.rows);
      CHECK(std::ranges::is_sorted(puzzle.rowIndices));
      if (puzzle.rows > 0) {
        CHECK_EQ(puzzle.rowIndices.front(), 0);
        CHECK_EQ(puzzle.rowIndices.back(), puzzle.rows - 1);
        for (const auto& rowIndex : puzzle.rowIndices) {
          CHECK(puzzle.isValidRowIndex(rowIndex));
        }
      }
      CHECK_FALSE(puzzle.isValidRowIndex(puzzle.rows + 1));
    }

    SUBCASE("Columns And Column Indices") {
      // Column indices are enough and sorted
      CHECK_EQ(puzzle.columns, puzzle.columnIndices.size());
      CHECK(std::ranges::is_sorted(puzzle.columnIndices));
      if (puzzle.columns > 0) {
        CHECK_EQ(puzzle.columnIndices.front(), 0);
        CHECK_EQ(puzzle.columnIndices.back(), puzzle.columns - 1);
        for (const auto& columnIndex : puzzle.columnIndices) {
          CHECK(puzzle.isValidColumnIndex(columnIndex));
        }
      }
      CHECK_FALSE(puzzle.isValidColumnIndex(puzzle.columns + 1));
    }

    SUBCASE("Digits") {
      // Digits are sorted
      CHECK(std::ranges::is_sorted(puzzle.digits));

      if (puzzle.digits.size() > 0) {
        CHECK_EQ(puzzle.digits.front(), 1);
        CHECK_EQ(puzzle.digits.back(), puzzle.digits.size());
        for (const auto& digit : puzzle.digits) {
          CHECK(puzzle.isValidDigit(digit));
        }
        CHECK_FALSE(puzzle.isValidDigit(puzzle.digits.back() + 1));
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
      CHECK_EQ(puzzle.allPossibilities.size(), possibilitiesCount);

      // All possibilities should appear in the correct order:
      std::size_t i = 0;
      for (const auto& rowIndex : puzzle.rowIndices) {
        for (const auto& columnIndex : puzzle.columnIndices) {
          for (const auto& digit : puzzle.digits) {
            const auto& cell = puzzle.allPossibilities[i];
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