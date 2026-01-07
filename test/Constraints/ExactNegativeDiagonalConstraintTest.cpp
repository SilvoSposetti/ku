
#include "ExactNegativeDiagonalConstraint.hpp"

#include "ConstraintTestHelpers.hpp"

#include <doctest.h>

TEST_SUITE("Constraints") {

  TEST_CASE("ExactNegativeDiagonalConstraint") {

    SUBCASE("Members") {
      ConstraintTestHelpers::memberChecks<ExactNegativeDiagonalConstraint<PuzzleIntrinsics<{0, 0, 0}>{}>>();
    }

    SUBCASE("Implicit Option Coverage") {
      // Cases with zero
      GENERATE_SUBCASE(ExactNegativeDiagonalConstraint, 0, 0, 0);
      // Regular cases
      GENERATE_SUBCASE(ExactNegativeDiagonalConstraint, 1, 1, 1);
      GENERATE_SUBCASE(ExactNegativeDiagonalConstraint, 2, 2, 2);
      GENERATE_SUBCASE(ExactNegativeDiagonalConstraint, 3, 3, 3);
      GENERATE_SUBCASE(ExactNegativeDiagonalConstraint, 7, 7, 7);
      GENERATE_SUBCASE(ExactNegativeDiagonalConstraint, 8, 8, 8);
      GENERATE_SUBCASE(ExactNegativeDiagonalConstraint, 13, 13, 13);
    }

    SUBCASE("Not supported") {
      // Digits don't match with size
      CHECK(!ExactNegativeDiagonalConstraint<PuzzleIntrinsics<{5, 5, 4}>{}>().supportsPuzzle());
      // Not square
      CHECK(!ExactNegativeDiagonalConstraint<PuzzleIntrinsics<{5, 4, 5}>{}>().supportsPuzzle());
      CHECK(!ExactNegativeDiagonalConstraint<PuzzleIntrinsics<{4, 5, 5}>{}>().supportsPuzzle());
    }

    SUBCASE("Explicit Options") {
      SUBCASE("2x2x2") {
        constexpr auto intrinsics = PuzzleIntrinsics<{2, 2, 2}>();
        ConstraintTestHelpers::checkConstraintOptions<intrinsics, 1, 0>(
            ExactNegativeDiagonalConstraint<intrinsics>(),
            2,
            std::vector<Option<1>>{{0}, {1}, {}, {}, {}, {}, {0}, {1}},
            0,
            {});
      }
      SUBCASE("5x5x5") {
        constexpr auto intrinsics = PuzzleIntrinsics<{5, 5, 5}>{};
        ConstraintTestHelpers::checkConstraintOptions<intrinsics, 1, 0>(
            ExactNegativeDiagonalConstraint<intrinsics>(),
            5,
            std::vector<Option<1>>{
                {0}, {1}, {2}, {3}, {4}, {}, {},  {},  {},  {},  {},  {},  {},  {},  {}, {},  {},  {},  {},  {},  {},
                {},  {},  {},  {},  {},  {}, {},  {},  {},  {0}, {1}, {2}, {3}, {4}, {}, {},  {},  {},  {},  {},  {},
                {},  {},  {},  {},  {},  {}, {},  {},  {},  {},  {},  {},  {},  {},  {}, {},  {},  {},  {0}, {1}, {2},
                {3}, {4}, {},  {},  {},  {}, {},  {},  {},  {},  {},  {},  {},  {},  {}, {},  {},  {},  {},  {},  {},
                {},  {},  {},  {},  {},  {}, {0}, {1}, {2}, {3}, {4}, {},  {},  {},  {}, {},  {},  {},  {},  {},  {},
                {},  {},  {},  {},  {},  {}, {},  {},  {},  {},  {},  {},  {},  {},  {}, {0}, {1}, {2}, {3}, {4},
            },
            0,
            {});
      }

      SUBCASE("9x9x9") {
        constexpr auto intrinsics = PuzzleIntrinsics<{9, 9, 9}>{};
        ConstraintTestHelpers::checkConstraintOptions<intrinsics, 1, 0>(
            ExactNegativeDiagonalConstraint<intrinsics>(),
            9,
            std::vector<Option<1>>{
                {0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
                {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
                {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
                {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
                {},  {},  {},  {},  {},  {},  {0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {},  {},  {},  {},  {},  {},
                {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
                {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
                {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
                {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8},
                {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
                {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
                {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
                {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {0}, {1}, {2},
                {3}, {4}, {5}, {6}, {7}, {8}, {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
                {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
                {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
                {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
                {},  {},  {},  {0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {},  {},  {},  {},  {},  {},  {},  {},  {},
                {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
                {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
                {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
                {},  {},  {},  {},  {},  {},  {},  {},  {},  {0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {},  {},  {},
                {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
                {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
                {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
                {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {0}, {1}, {2}, {3}, {4}, {5},
                {6}, {7}, {8}, {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
                {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
                {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
                {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
                {0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
                {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
                {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
                {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
                {},  {},  {},  {},  {},  {},  {0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8},
            },
            0,
            {});
      }
    }
  }
}
