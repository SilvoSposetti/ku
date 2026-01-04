#include "ConstraintType.hpp"

#include <doctest.h>

TEST_CASE("Constraint Type") {

  SUBCASE("Bitwise operations") {
    constexpr auto simple = ConstraintType::SUDOKU_CELL;
    CHECK(static_cast<bool>(simple & ConstraintType::SUDOKU_CELL));
    CHECK_FALSE(static_cast<bool>(simple & ConstraintType::SUDOKU_COLUMN));
    CHECK_FALSE(static_cast<bool>(simple & ConstraintType::SUDOKU_ROW));
    CHECK_FALSE(static_cast<bool>(simple & ConstraintType::SUDOKU_BOX));
    CHECK_EQ(simple | ConstraintType::SUDOKU_CELL, simple);

    constexpr auto sudoku =
        simple | ConstraintType::SUDOKU_COLUMN | ConstraintType::SUDOKU_ROW | ConstraintType::SUDOKU_BOX;

    CHECK(static_cast<bool>(sudoku & ConstraintType::SUDOKU_CELL));
    CHECK(static_cast<bool>(sudoku & ConstraintType::SUDOKU_COLUMN));
    CHECK(static_cast<bool>(sudoku & ConstraintType::SUDOKU_ROW));
    CHECK(static_cast<bool>(sudoku & ConstraintType::SUDOKU_BOX));
    CHECK_FALSE(static_cast<bool>(sudoku & ConstraintType::POSITIVE_DIAGONAL));
    CHECK_FALSE(static_cast<bool>(sudoku & ConstraintType::NEGATIVE_DIAGONAL));
    CHECK_EQ(sudoku | ConstraintType::SUDOKU_CELL, sudoku);
    CHECK_EQ(sudoku | ConstraintType::SUDOKU_COLUMN, sudoku);
    CHECK_EQ(sudoku | ConstraintType::SUDOKU_ROW, sudoku);
    CHECK_EQ(sudoku | ConstraintType::SUDOKU_BOX, sudoku);
  }
}
