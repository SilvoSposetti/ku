#include "ConstraintFactory.hpp"

#include <doctest.h>

TEST_CASE("Constraint Factory") {
  constexpr auto intrinsics0x0x0 = PuzzleIntrinsics<{0, 0, 0}>{};
  constexpr auto intrinsics1x1x1 = PuzzleIntrinsics<{1, 1, 1}>{};
  constexpr auto intrinsics9x9x9 = PuzzleIntrinsics<{9, 9, 9}>{};
  constexpr auto intrinsics5x9x9 = PuzzleIntrinsics<{5, 9, 9}>{};
  constexpr auto intrinsics9x5x9 = PuzzleIntrinsics<{9, 5, 9}>{};

  SUBCASE("Cell") {
    constexpr auto constraint = ConstraintType::CELL;
    CHECK_NOTHROW(ConstraintFactory::makeConstraint<intrinsics0x0x0>(constraint));
    CHECK_NOTHROW(ConstraintFactory::makeConstraint<intrinsics1x1x1>(constraint));
    CHECK_NOTHROW(ConstraintFactory::makeConstraint<intrinsics9x9x9>(constraint));
    CHECK_NOTHROW(ConstraintFactory::makeConstraint<intrinsics5x9x9>(constraint));
    CHECK_NOTHROW(ConstraintFactory::makeConstraint<intrinsics9x5x9>(constraint));
  }

  SUBCASE("Exact row") {
    constexpr auto constraint = ConstraintType::EXACT_ROW;
    CHECK_NOTHROW(ConstraintFactory::makeConstraint<intrinsics0x0x0>(constraint));
    CHECK_NOTHROW(ConstraintFactory::makeConstraint<intrinsics1x1x1>(constraint));
    CHECK_NOTHROW(ConstraintFactory::makeConstraint<intrinsics9x9x9>(constraint));
    CHECK_NOTHROW(ConstraintFactory::makeConstraint<intrinsics5x9x9>(constraint));
    CHECK_THROWS(ConstraintFactory::makeConstraint<intrinsics9x5x9>(constraint));
  }

  SUBCASE("Exact column") {
    constexpr auto constraint = ConstraintType::EXACT_COLUMN;
    CHECK_NOTHROW(ConstraintFactory::makeConstraint<intrinsics0x0x0>(constraint));
    CHECK_NOTHROW(ConstraintFactory::makeConstraint<intrinsics1x1x1>(constraint));
    CHECK_NOTHROW(ConstraintFactory::makeConstraint<intrinsics9x9x9>(constraint));
    CHECK_THROWS(ConstraintFactory::makeConstraint<intrinsics5x9x9>(constraint));
    CHECK_NOTHROW(ConstraintFactory::makeConstraint<intrinsics9x5x9>(constraint));
  }

  SUBCASE("Exact 3x3 boxes") {
    constexpr auto constraint = ConstraintType::EXACT_3x3_BOXES;
    CHECK_THROWS(ConstraintFactory::makeConstraint<intrinsics0x0x0>(constraint));
    CHECK_THROWS(ConstraintFactory::makeConstraint<intrinsics1x1x1>(constraint));
    CHECK_NOTHROW(ConstraintFactory::makeConstraint<intrinsics9x9x9>(constraint));
    CHECK_THROWS(ConstraintFactory::makeConstraint<intrinsics5x9x9>(constraint));
    CHECK_THROWS(ConstraintFactory::makeConstraint<intrinsics9x5x9>(constraint));
  }

  SUBCASE("Positive Diagonal") {
    constexpr auto constraint = ConstraintType::POSITIVE_DIAGONAL;
    CHECK_NOTHROW(ConstraintFactory::makeConstraint<intrinsics0x0x0>(constraint));
    CHECK_NOTHROW(ConstraintFactory::makeConstraint<intrinsics1x1x1>(constraint));
    CHECK_NOTHROW(ConstraintFactory::makeConstraint<intrinsics9x9x9>(constraint));
    CHECK_THROWS(ConstraintFactory::makeConstraint<intrinsics5x9x9>(constraint));
    CHECK_THROWS(ConstraintFactory::makeConstraint<intrinsics9x5x9>(constraint));
  }

  SUBCASE("Positive Diagonal Even") {
    constexpr auto constraint = ConstraintType::POSITIVE_DIAGONAL_EVEN;
    CHECK_THROWS(ConstraintFactory::makeConstraint<intrinsics0x0x0>(constraint));
    CHECK_THROWS(ConstraintFactory::makeConstraint<intrinsics1x1x1>(constraint));
    CHECK_NOTHROW(ConstraintFactory::makeConstraint<intrinsics9x9x9>(constraint));
    CHECK_THROWS(ConstraintFactory::makeConstraint<intrinsics5x9x9>(constraint));
    CHECK_THROWS(ConstraintFactory::makeConstraint<intrinsics9x5x9>(constraint));
  }

  SUBCASE("Positive Diagonal Odd") {
    constexpr auto constraint = ConstraintType::POSITIVE_DIAGONAL_ODD;
    CHECK_THROWS(ConstraintFactory::makeConstraint<intrinsics0x0x0>(constraint));
    CHECK_NOTHROW(ConstraintFactory::makeConstraint<intrinsics1x1x1>(constraint));
    CHECK_NOTHROW(ConstraintFactory::makeConstraint<intrinsics9x9x9>(constraint));
    CHECK_THROWS(ConstraintFactory::makeConstraint<intrinsics5x9x9>(constraint));
    CHECK_THROWS(ConstraintFactory::makeConstraint<intrinsics9x5x9>(constraint));
  }

  SUBCASE("Negative Diagonal") {
    constexpr auto constraint = ConstraintType::NEGATIVE_DIAGONAL;
    CHECK_NOTHROW(ConstraintFactory::makeConstraint<intrinsics0x0x0>(constraint));
    CHECK_NOTHROW(ConstraintFactory::makeConstraint<intrinsics1x1x1>(constraint));
    CHECK_NOTHROW(ConstraintFactory::makeConstraint<intrinsics9x9x9>(constraint));
    CHECK_THROWS(ConstraintFactory::makeConstraint<intrinsics5x9x9>(constraint));
    CHECK_THROWS(ConstraintFactory::makeConstraint<intrinsics9x5x9>(constraint));
  }

  SUBCASE("Negative Diagonal Even") {
    constexpr auto constraint = ConstraintType::NEGATIVE_DIAGONAL_EVEN;
    CHECK_THROWS(ConstraintFactory::makeConstraint<intrinsics0x0x0>(constraint));
    CHECK_THROWS(ConstraintFactory::makeConstraint<intrinsics1x1x1>(constraint));
    CHECK_NOTHROW(ConstraintFactory::makeConstraint<intrinsics9x9x9>(constraint));
    CHECK_THROWS(ConstraintFactory::makeConstraint<intrinsics5x9x9>(constraint));
    CHECK_THROWS(ConstraintFactory::makeConstraint<intrinsics9x5x9>(constraint));
  }

  SUBCASE("Negative Diagonal Odd") {
    constexpr auto constraint = ConstraintType::NEGATIVE_DIAGONAL_ODD;
    CHECK_THROWS(ConstraintFactory::makeConstraint<intrinsics0x0x0>(constraint));
    CHECK_NOTHROW(ConstraintFactory::makeConstraint<intrinsics1x1x1>(constraint));
    CHECK_NOTHROW(ConstraintFactory::makeConstraint<intrinsics9x9x9>(constraint));
    CHECK_THROWS(ConstraintFactory::makeConstraint<intrinsics5x9x9>(constraint));
    CHECK_THROWS(ConstraintFactory::makeConstraint<intrinsics9x5x9>(constraint));
  }

  SUBCASE("Hyper Sudoku") {
    constexpr auto constraint = ConstraintType::HYPER_SUDOKU;
    CHECK_THROWS(ConstraintFactory::makeConstraint<intrinsics0x0x0>(constraint));
    CHECK_THROWS(ConstraintFactory::makeConstraint<intrinsics1x1x1>(constraint));
    CHECK_NOTHROW(ConstraintFactory::makeConstraint<intrinsics9x9x9>(constraint));
    CHECK_THROWS(ConstraintFactory::makeConstraint<intrinsics5x9x9>(constraint));
    CHECK_THROWS(ConstraintFactory::makeConstraint<intrinsics9x5x9>(constraint));
  }

  SUBCASE("Disjoint Boxes") {
    constexpr auto constraint = ConstraintType::DISJOINT_BOXES;
    CHECK_THROWS(ConstraintFactory::makeConstraint<intrinsics0x0x0>(constraint));
    CHECK_THROWS(ConstraintFactory::makeConstraint<intrinsics1x1x1>(constraint));
    CHECK_NOTHROW(ConstraintFactory::makeConstraint<intrinsics9x9x9>(constraint));
    CHECK_THROWS(ConstraintFactory::makeConstraint<intrinsics5x9x9>(constraint));
    CHECK_THROWS(ConstraintFactory::makeConstraint<intrinsics9x5x9>(constraint));
  }

  SUBCASE("Asterisk") {
    constexpr auto constraint = ConstraintType::ASTERISK;
    CHECK_THROWS(ConstraintFactory::makeConstraint<intrinsics0x0x0>(constraint));
    CHECK_THROWS(ConstraintFactory::makeConstraint<intrinsics1x1x1>(constraint));
    CHECK_NOTHROW(ConstraintFactory::makeConstraint<intrinsics9x9x9>(constraint));
    CHECK_THROWS(ConstraintFactory::makeConstraint<intrinsics5x9x9>(constraint));
    CHECK_THROWS(ConstraintFactory::makeConstraint<intrinsics9x5x9>(constraint));
  }

  SUBCASE("King Torus Pattern") {
    constexpr auto constraint = ConstraintType::KING_TORUS_PATTERN;
    CHECK_THROWS(ConstraintFactory::makeConstraint<intrinsics0x0x0>(constraint));
    CHECK_THROWS(ConstraintFactory::makeConstraint<intrinsics1x1x1>(constraint));
    CHECK_NOTHROW(ConstraintFactory::makeConstraint<intrinsics9x9x9>(constraint));
    CHECK_NOTHROW(ConstraintFactory::makeConstraint<intrinsics5x9x9>(constraint));
    CHECK_NOTHROW(ConstraintFactory::makeConstraint<intrinsics9x5x9>(constraint));
  }

  SUBCASE("Knight Torus Pattern") {
    constexpr auto constraint = ConstraintType::KNIGHT_TORUS_PATTERN;
    CHECK_THROWS(ConstraintFactory::makeConstraint<intrinsics0x0x0>(constraint));
    CHECK_THROWS(ConstraintFactory::makeConstraint<intrinsics1x1x1>(constraint));
    CHECK_NOTHROW(ConstraintFactory::makeConstraint<intrinsics9x9x9>(constraint));
    CHECK_NOTHROW(ConstraintFactory::makeConstraint<intrinsics5x9x9>(constraint));
    CHECK_NOTHROW(ConstraintFactory::makeConstraint<intrinsics9x5x9>(constraint));
  }

  SUBCASE("King Pattern") {
    constexpr auto constraint = ConstraintType::KING_PATTERN;
    CHECK_THROWS(ConstraintFactory::makeConstraint<intrinsics0x0x0>(constraint));
    CHECK_THROWS(ConstraintFactory::makeConstraint<intrinsics1x1x1>(constraint));
    CHECK_NOTHROW(ConstraintFactory::makeConstraint<intrinsics9x9x9>(constraint));
    CHECK_NOTHROW(ConstraintFactory::makeConstraint<intrinsics5x9x9>(constraint));
    CHECK_NOTHROW(ConstraintFactory::makeConstraint<intrinsics9x5x9>(constraint));
  }

  SUBCASE("Knight Pattern") {
    constexpr auto constraint = ConstraintType::KNIGHT_PATTERN;
    CHECK_THROWS(ConstraintFactory::makeConstraint<intrinsics0x0x0>(constraint));
    CHECK_THROWS(ConstraintFactory::makeConstraint<intrinsics1x1x1>(constraint));
    CHECK_NOTHROW(ConstraintFactory::makeConstraint<intrinsics9x9x9>(constraint));
    CHECK_NOTHROW(ConstraintFactory::makeConstraint<intrinsics5x9x9>(constraint));
    CHECK_NOTHROW(ConstraintFactory::makeConstraint<intrinsics9x5x9>(constraint));
  }
}
