#include "constraintTemplated/utilities/NeighborPatternUtilities.hpp"

#include <doctest.h>

/** Templated function to help with the checks.
 * @tparam puzzle The puzzle
 * @tparam patternSize The size of the pattern
 * @tparam expectedOptionSize The expected size for the computed option
 * @param rowIndex The row index of the cell
 * @param columnIndex The column index of the cell
 * @param digit The digit of the cell
 * @param pattern The array containing the unique pattern used to generate the option
 * @param expectedOption The expected option
 */
template <PuzzleIntrinsics puzzle, std::size_t patternSize, std::size_t expectedOptionSize>
constexpr void check(uint32_t rowIndex,
                     uint32_t columnIndex,
                     uint32_t digit,
                     const std::array<std::pair<int32_t, int32_t>, patternSize>& pattern,
                     const Option<expectedOptionSize>& expectedOption) {
  CHECK_EQ(NeighborPatternUtilities::computeNeighborPatternTorusOption<puzzle, patternSize, expectedOptionSize>(
               rowIndex, columnIndex, digit, pattern),
           expectedOption);
}

TEST_CASE("Neighbor Pattern Utilities") {
  constexpr std::size_t optionSize = 8;
  constexpr std::size_t neighborsSize = 4;

  SUBCASE("Moore Neighborhood, Torus, 3x3") {
    constexpr auto puzzle = PuzzleIntrinsics<{3, 3, 9}>();
    constexpr auto moorePattern =
        std::array<std::pair<int32_t, int32_t>, neighborsSize>{std::make_pair(1, 0), {1, -1}, {0, -1}, {-1, -1}};

    check<puzzle, neighborsSize, optionSize>(0, 0, 1, moorePattern, {0, 9, 18, 27, 90, 108, 189, 315});
    check<puzzle, neighborsSize, optionSize>(0, 0, 5, moorePattern, {4, 13, 22, 31, 94, 112, 193, 319});
    check<puzzle, neighborsSize, optionSize>(0, 1, 4, moorePattern, {21, 39, 48, 57, 66, 120, 147, 246});
    check<puzzle, neighborsSize, optionSize>(2, 0, 3, moorePattern, {2, 83, 209, 218, 227, 236, 245, 308});
    check<puzzle, neighborsSize, optionSize>(1, 1, 8, moorePattern, {34, 133, 151, 160, 169, 178, 232, 259});
    check<puzzle, neighborsSize, optionSize>(2, 2, 2, moorePattern, {46, 73, 172, 271, 289, 298, 307, 316});
  }

  SUBCASE("Knight Neighborhood, Torus  5x5") {
    constexpr auto puzzle = PuzzleIntrinsics<{5, 5, 9}>();
    constexpr auto knightPattern =
        std::array<std::pair<int32_t, int32_t>, neighborsSize>{std::make_pair(1, 0), {1, -1}, {0, -1}, {-1, -1}};
    constexpr std::size_t optionSize = 8;

    check<puzzle, neighborsSize, optionSize>(0, 0, 1, knightPattern, {0, 9, 18, 27, 162, 180, 333, 891});
    check<puzzle, neighborsSize, optionSize>(0, 0, 5, knightPattern, {4, 13, 22, 31, 166, 184, 337, 895});
    check<puzzle, neighborsSize, optionSize>(0, 3, 4, knightPattern, {93, 111, 120, 129, 138, 264, 291, 822});
    check<puzzle, neighborsSize, optionSize>(2, 2, 1, knightPattern, {243, 414, 432, 441, 450, 459, 585, 612});
    check<puzzle, neighborsSize, optionSize>(1, 3, 5, knightPattern, {103, 274, 292, 301, 310, 319, 445, 472});
    check<puzzle, neighborsSize, optionSize>(4, 4, 2, knightPattern, {118, 145, 676, 847, 865, 874, 883, 892});
    check<puzzle, neighborsSize, optionSize>(3, 4, 7, knightPattern, {501, 672, 690, 699, 708, 717, 843, 870});
  }

  SUBCASE("Cross Neighborhood, Torus  5x5") {
    constexpr auto puzzle = PuzzleIntrinsics<{5, 5, 9}>();
    constexpr auto crossPattern =
        std::array<std::pair<int32_t, int32_t>, neighborsSize>{std::make_pair(1, 1), {2, 2}, {1, -1}, {2, -2}};
    constexpr std::size_t optionSize = 8;

    check<puzzle, neighborsSize, optionSize>(0, 0, 1, crossPattern, {0, 9, 18, 27, 216, 342, 441, 495});
    check<puzzle, neighborsSize, optionSize>(0, 0, 5, crossPattern, {4, 13, 22, 31, 220, 346, 445, 499});
    check<puzzle, neighborsSize, optionSize>(0, 3, 4, crossPattern, {111, 120, 129, 138, 273, 327, 372, 426});
    check<puzzle, neighborsSize, optionSize>(1, 3, 5, crossPattern, {292, 301, 310, 319, 454, 508, 553, 607});
    check<puzzle, neighborsSize, optionSize>(2, 2, 1, crossPattern, {432, 441, 450, 459, 594, 648, 747, 873});
    check<puzzle, neighborsSize, optionSize>(4, 4, 2, crossPattern, {1, 127, 226, 280, 865, 874, 883, 892});
    check<puzzle, neighborsSize, optionSize>(3, 4, 7, crossPattern, {51, 105, 690, 699, 708, 717, 726, 852});
  }
}