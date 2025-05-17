#include "puzzles/PuzzleUtilities.hpp"

#include <doctest.h>

TEST_CASE("Puzzle Utilities") {

  SUBCASE("isOnPositiveDiagonal") {
    // No rows or columns
    CHECK_FALSE(PuzzleUtilities::isOnPositiveDiagonal<PuzzleIntrinsics<{0, 0, 5}>{}>(0, 0));
    CHECK_FALSE(PuzzleUtilities::isOnPositiveDiagonal<PuzzleIntrinsics<{0, 1, 5}>{}>(0, 0));
    CHECK_FALSE(PuzzleUtilities::isOnPositiveDiagonal<PuzzleIntrinsics<{1, 0, 5}>{}>(0, 0));
    // Puzzle is not a square
    CHECK_FALSE(PuzzleUtilities::isOnPositiveDiagonal<PuzzleIntrinsics<{8, 7, 5}>{}>(0, 0));
    CHECK_FALSE(PuzzleUtilities::isOnPositiveDiagonal<PuzzleIntrinsics<{7, 8, 5}>{}>(0, 0));
    // Is outside of puzzle
    CHECK_FALSE(PuzzleUtilities::isOnPositiveDiagonal<PuzzleIntrinsics<{5, 5, 5}>{}>(7, 7));
    // Not on diagonal
    CHECK_FALSE(PuzzleUtilities::isOnPositiveDiagonal<PuzzleIntrinsics<{7, 7, 5}>{}>(1, 2));
    CHECK_FALSE(PuzzleUtilities::isOnPositiveDiagonal<PuzzleIntrinsics<{7, 7, 5}>{}>(2, 0));
    // Valid
    CHECK(PuzzleUtilities::isOnNegativeDiagonal<PuzzleIntrinsics<{1, 1, 5}>{}>(0, 0));
    CHECK(PuzzleUtilities::isOnPositiveDiagonal<PuzzleIntrinsics<{7, 7, 5}>{}>(0, 6));
    CHECK(PuzzleUtilities::isOnPositiveDiagonal<PuzzleIntrinsics<{7, 7, 5}>{}>(6, 0));
    CHECK(PuzzleUtilities::isOnPositiveDiagonal<PuzzleIntrinsics<{5, 5, 5}>{}>(2, 2));
    CHECK(PuzzleUtilities::isOnPositiveDiagonal<PuzzleIntrinsics<{12, 12, 5}>{}>(4, 7));
    CHECK(PuzzleUtilities::isOnPositiveDiagonal<PuzzleIntrinsics<{12, 12, 5}>{}>(7, 4));
  }

  SUBCASE("IsOnNegativeDiagonal") {
    // No rows or columns
    CHECK_FALSE(PuzzleUtilities::isOnNegativeDiagonal<PuzzleIntrinsics<{0, 0, 5}>{}>(0, 0));
    CHECK_FALSE(PuzzleUtilities::isOnNegativeDiagonal<PuzzleIntrinsics<{0, 1, 5}>{}>(0, 0));
    CHECK_FALSE(PuzzleUtilities::isOnNegativeDiagonal<PuzzleIntrinsics<{1, 0, 5}>{}>(0, 0));
    // Puzzle is not a square
    CHECK_FALSE(PuzzleUtilities::isOnNegativeDiagonal<PuzzleIntrinsics<{8, 7, 5}>{}>(0, 0));
    CHECK_FALSE(PuzzleUtilities::isOnNegativeDiagonal<PuzzleIntrinsics<{7, 8, 5}>{}>(0, 0));
    // Is outside of puzzle
    CHECK_FALSE(PuzzleUtilities::isOnNegativeDiagonal<PuzzleIntrinsics<{5, 5, 5}>{}>(7, 7));
    // Not on diagonal
    CHECK_FALSE(PuzzleUtilities::isOnNegativeDiagonal<PuzzleIntrinsics<{7, 7, 5}>{}>(1, 2));
    CHECK_FALSE(PuzzleUtilities::isOnNegativeDiagonal<PuzzleIntrinsics<{7, 7, 5}>{}>(2, 0));
    // Valid
    CHECK(PuzzleUtilities::isOnNegativeDiagonal<PuzzleIntrinsics<{1, 1, 5}>{}>(0, 0));
    CHECK(PuzzleUtilities::isOnNegativeDiagonal<PuzzleIntrinsics<{7, 7, 5}>{}>(1, 1));
    CHECK(PuzzleUtilities::isOnNegativeDiagonal<PuzzleIntrinsics<{5, 5, 5}>{}>(0, 0));
    CHECK(PuzzleUtilities::isOnNegativeDiagonal<PuzzleIntrinsics<{12, 12, 5}>{}>(7, 7));
  }
}