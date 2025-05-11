
#include "constraintTemplated/Constraint.hpp"

#include "ConstraintTestHelpers.hpp"
#include "utilities/IdPacking.hpp"

#include <doctest.h>

template <PuzzleIntrinsics puzzleIntrinsics>
struct ExampleConstraint {

  constexpr static bool supportsPuzzle() {
    return true;
  }

  constexpr static Option primaryOption(Index row, Index column, Digit digit) {
    return Option{IdPacking::packId(
        row, column, digit - 1, puzzleIntrinsics.rows, puzzleIntrinsics.columns, puzzleIntrinsics.digits.size())};
  }

  constexpr static Option secondaryOption(Index row, Index column, Digit digit) {
    constexpr auto total =
        static_cast<uint32_t>(puzzleIntrinsics.rows * puzzleIntrinsics.columns * puzzleIntrinsics.digits.size());
    return Option{
        (total - 1) -
        IdPacking::packId(
            row, column, digit - 1, puzzleIntrinsics.rows, puzzleIntrinsics.columns, puzzleIntrinsics.digits.size())};
  }
};

TEST_CASE("Base Constraint") {

  constexpr auto puzzleIntrinsics = PuzzleIntrinsics<{2, 2, 2}>();
  constexpr auto constraint =
      Constraint<ExampleConstraint<puzzleIntrinsics>, puzzleIntrinsics>(ConstraintType::NONE, "Name", "Description");

  CHECK_EQ(constraint.type, ConstraintType::NONE);
  CHECK_EQ(constraint.name, "Name");
  CHECK_EQ(constraint.description, "Description");
  checkConstraintOptions(constraint,
                         8,
                         std::vector<Option>{{0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}},
                         8,
                         std::vector<Option>{{7}, {6}, {5}, {4}, {3}, {2}, {1}, {0}});
}
