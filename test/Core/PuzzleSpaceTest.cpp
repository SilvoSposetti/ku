#include "PuzzleSpace.hpp"

#include <doctest.h>

TEST_CASE("Puzzle Space") {

  SUBCASE("Construction") {
    constexpr auto space1 = PuzzleSpace(0, 0, 0);
    CHECK_EQ(space1.rowsCount, 0);
    CHECK_EQ(space1.columnsCount, 0);
    CHECK_EQ(space1.digitsCount, 0);

    constexpr auto space2 = PuzzleSpace(1, 2, 3);
    CHECK_EQ(space2.rowsCount, 1);
    CHECK_EQ(space2.columnsCount, 2);
    CHECK_EQ(space2.digitsCount, 3);
  }
}
