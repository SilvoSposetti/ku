
#include "PositiveDiagonalEvenConstraint.hpp"

#include "ConstraintTestHelpers.hpp"

#include <doctest.h>

TEST_SUITE("Constraints") {

  TEST_CASE("PositiveDiagonalEvenConstraint") {

    SUBCASE("Members") {
      ConstraintTestHelpers::memberChecks<PositiveDiagonalEvenConstraint<PuzzleIntrinsics<{0, 0, 0}>{}>>();
    }

    SUBCASE("Implicit Option Coverage") {
      GENERATE_SUBCASE(PositiveDiagonalEvenConstraint, 2, 2, 5);
      GENERATE_SUBCASE(PositiveDiagonalEvenConstraint, 3, 3, 3);
      GENERATE_SUBCASE(PositiveDiagonalEvenConstraint, 7, 7, 4);
      GENERATE_SUBCASE(PositiveDiagonalEvenConstraint, 8, 8, 2);
    }

    SUBCASE("Not Supported") {
      // No even digits
      CHECK(!PositiveDiagonalEvenConstraint<PuzzleIntrinsics<{5, 5, 0}>{}>().supportsPuzzle());
      CHECK(!PositiveDiagonalEvenConstraint<PuzzleIntrinsics<{5, 5, 1}>{}>().supportsPuzzle());
      // Not square
      CHECK(!PositiveDiagonalEvenConstraint<PuzzleIntrinsics<{1, 3, 5}>{}>().supportsPuzzle());
      CHECK(!PositiveDiagonalEvenConstraint<PuzzleIntrinsics<{6, 4, 5}>{}>().supportsPuzzle());
    }

    SUBCASE("Explicit Options") {
      SUBCASE("2x2x0") {
        constexpr auto intrinsics = PuzzleIntrinsics<{2, 2, 0}>();
        ConstraintTestHelpers::checkConstraintOptions<intrinsics, 1, 0>(
            PositiveDiagonalEvenConstraint<intrinsics>(), 0, {}, 0, {});
      }

      SUBCASE("2x2x2") {
        constexpr auto intrinsics = PuzzleIntrinsics<{2, 2, 2}>();
        ConstraintTestHelpers::checkConstraintOptions<intrinsics, 1, 0>(PositiveDiagonalEvenConstraint<intrinsics>(),
                                                                        2,
                                                                        std::vector<Option<1>>{
                                                                            {},
                                                                            {},
                                                                            {},
                                                                            {0},
                                                                            {},
                                                                            {1},
                                                                            {},
                                                                            {},
                                                                        },
                                                                        0,
                                                                        {});
      }

      SUBCASE("5x5x7") {
        constexpr auto intrinsics = PuzzleIntrinsics<{5, 5, 7}>{};
        ConstraintTestHelpers::checkConstraintOptions<intrinsics, 1, 0>(
            PositiveDiagonalEvenConstraint<intrinsics>(),
            5,
            std::vector<Option<1>>{
                {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},
                {}, {},  {}, {},  {}, {},  {}, {0}, {}, {0}, {}, {0}, {}, {},  {}, {},  {}, {},  {}, {},  {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {1}, {}, {1}, {}, {1}, {}, {},  {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {2}, {}, {2},
                {}, {2}, {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},
                {}, {},  {}, {3}, {}, {3}, {}, {3}, {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {}, {4}, {}, {4}, {}, {4}, {}, {},  {}, {},  {}, {},  {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {},
            },
            0,
            {});
      }

      SUBCASE("9x9x9") {
        constexpr auto intrinsics = PuzzleIntrinsics<{9, 9, 9}>{};
        ConstraintTestHelpers::checkConstraintOptions<intrinsics, 1, 0>(
            PositiveDiagonalEvenConstraint<intrinsics>(),
            9,
            std::vector<Option<1>>{
                {}, {},  {}, {},  {}, {},  {}, {},  {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
                {}, {0}, {}, {0}, {}, {0}, {}, {0}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
                {}, {1}, {}, {1}, {}, {1}, {}, {1}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
                {}, {2}, {}, {2}, {}, {2}, {}, {2}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
                {}, {3}, {}, {3}, {}, {3}, {}, {3}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
                {}, {4}, {}, {4}, {}, {4}, {}, {4}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
                {}, {5}, {}, {5}, {}, {5}, {}, {5}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
                {}, {6}, {}, {6}, {}, {6}, {}, {6}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
                {}, {7}, {}, {7}, {}, {7}, {}, {7}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
                {}, {8}, {}, {8}, {}, {8}, {}, {8}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {},
            },
            0,
            {});
      }
    }
  }
}
