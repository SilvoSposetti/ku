#include "puzzles/Cell.hpp"

#include <doctest.h>

TEST_CASE("Cell") {

  SUBCASE("Default constructor") {
    constexpr Cell cell;
    CHECK_EQ(cell.rowIndex, 0);
    CHECK_EQ(cell.columnIndex, 0);
    CHECK_EQ(cell.digit, Digits::invalidDigit);
  }

  SUBCASE("Constructor") {
    constexpr auto cell = Cell(0, 1, 2);
    CHECK_EQ(cell.rowIndex, 0);
    CHECK_EQ(cell.columnIndex, 1);
    CHECK_EQ(cell.digit, 2);
  }
}