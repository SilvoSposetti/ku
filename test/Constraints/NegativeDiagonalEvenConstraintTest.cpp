
#include "NegativeDiagonalEvenConstraint.hpp"

#include "ConstraintTestHelpers.hpp"

#include <doctest.h>

TEST_SUITE("Constraints") {

  TEST_CASE("NegativeDiagonalEvenConstraint") {

    SUBCASE("Members") {
      ConstraintTestHelpers::memberChecks<NegativeDiagonalEvenConstraint<PuzzleIntrinsics<{0, 0, 0}>{}>>();
    }

    SUBCASE("Implicit Option Coverage") {
      GENERATE_SUBCASE(NegativeDiagonalEvenConstraint, 2, 2, 5);
      GENERATE_SUBCASE(NegativeDiagonalEvenConstraint, 3, 3, 3);
      GENERATE_SUBCASE(NegativeDiagonalEvenConstraint, 7, 7, 4);
      GENERATE_SUBCASE(NegativeDiagonalEvenConstraint, 8, 8, 2);
    }

    SUBCASE("Not Supported") {
      // No even digits
      CHECK(!NegativeDiagonalEvenConstraint<PuzzleIntrinsics<{5, 5, 0}>{}>().supportsPuzzle());
      CHECK(!NegativeDiagonalEvenConstraint<PuzzleIntrinsics<{5, 5, 1}>{}>().supportsPuzzle());
      // Not square
      CHECK(!NegativeDiagonalEvenConstraint<PuzzleIntrinsics<{1, 3, 5}>{}>().supportsPuzzle());
      CHECK(!NegativeDiagonalEvenConstraint<PuzzleIntrinsics<{6, 4, 5}>{}>().supportsPuzzle());
    }

    SUBCASE("Explicit Options") {
      SUBCASE("2x2x0") {
        constexpr auto intrinsics = PuzzleIntrinsics<{2, 2, 0}>();
        ConstraintTestHelpers::checkConstraintOptions<intrinsics, 1, 0>(
            NegativeDiagonalEvenConstraint<intrinsics>(), 0, {}, 0, {});
      }

      SUBCASE("2x2x2") {
        constexpr auto intrinsics = PuzzleIntrinsics<{2, 2, 2}>();
        ConstraintTestHelpers::checkConstraintOptions<intrinsics, 1, 0>(NegativeDiagonalEvenConstraint<intrinsics>(),
                                                                        2,
                                                                        std::vector<Option<1>>{
                                                                            {},
                                                                            {0},
                                                                            {},
                                                                            {},
                                                                            {},
                                                                            {},
                                                                            {},
                                                                            {1},
                                                                        },
                                                                        0,
                                                                        {});
      }

      SUBCASE("5x5x7") {
        constexpr auto intrinsics = PuzzleIntrinsics<{5, 5, 7}>{};
        ConstraintTestHelpers::checkConstraintOptions<intrinsics, 1, 0>(
            NegativeDiagonalEvenConstraint<intrinsics>(),
            5,
            std::vector<Option<1>>{
                {}, {0}, {}, {0}, {}, {0}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},  {}, {},  {}, {},  {}, {},
                {}, {},  {}, {},  {}, {},  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},  {}, {},  {}, {},  {}, {1},
                {}, {1}, {}, {1}, {}, {},  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},  {}, {},  {}, {},  {}, {},
                {}, {},  {}, {},  {}, {},  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},  {}, {},  {}, {2}, {}, {2},
                {}, {2}, {}, {},  {}, {},  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},  {}, {},  {}, {},  {}, {},
                {}, {},  {}, {},  {}, {},  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},  {}, {3}, {}, {3}, {}, {3},
                {}, {},  {}, {},  {}, {},  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},  {}, {},  {}, {},  {}, {},
                {}, {},  {}, {},  {}, {},  {}, {}, {}, {}, {}, {}, {}, {}, {}, {4}, {}, {4}, {}, {4}, {},
            },
            0,
            {});
      }

      SUBCASE("9x9x9") {
        constexpr auto intrinsics = PuzzleIntrinsics<{9, 9, 9}>{};
        ConstraintTestHelpers::checkConstraintOptions<intrinsics, 1, 0>(
            NegativeDiagonalEvenConstraint<intrinsics>(),
            9,
            std::vector<Option<1>>{
                {}, {0}, {}, {0}, {}, {0}, {}, {0}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {1},
                {}, {1}, {}, {1}, {}, {1}, {}, {},  {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {2}, {}, {2},
                {}, {2}, {}, {2}, {}, {},  {}, {},  {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {3}, {}, {3}, {}, {3},
                {}, {3}, {}, {},  {}, {},  {}, {},  {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {},  {}, {},  {}, {},  {}, {},  {}, {4}, {}, {4}, {}, {4}, {}, {4},
                {}, {},  {}, {},  {}, {},  {}, {},  {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {},  {}, {},  {}, {},  {}, {5}, {}, {5}, {}, {5}, {}, {5}, {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {},  {}, {},  {}, {6}, {}, {6}, {}, {6}, {}, {6}, {}, {},  {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {},  {}, {7}, {}, {7}, {}, {7}, {}, {7}, {}, {},  {}, {},  {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},
                {}, {},  {}, {},  {}, {},  {}, {},  {8}, {}, {8}, {}, {8}, {}, {8}, {},
            },
            0,
            {});
      }
    }
  }
}
