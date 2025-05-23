
#include "constraintTemplated/Constraint.hpp"

#include "ConstraintTestHelpers.hpp"
#include "constraintTemplated/ConstraintConcept.hpp"
#include "utilities/IdPacking.hpp"

#include <doctest.h>

namespace ExampleConstraintOptionSizes {
static constexpr std::size_t primary = 1;
static constexpr std::size_t secondary = 1;
} // namespace ExampleConstraintOptionSizes

template <PuzzleIntrinsics puzzle>
struct ExampleConstraint : public Constraint<ExampleConstraint<puzzle>,
                                             puzzle,
                                             ExampleConstraintOptionSizes::primary,
                                             ExampleConstraintOptionSizes::secondary> {
public:
  constexpr ExampleConstraint()
      : Constraint<ExampleConstraint<puzzle>,
                   puzzle,
                   ExampleConstraintOptionSizes::primary,
                   ExampleConstraintOptionSizes::secondary>(
            ConstraintType::NONE, "Name", "Description") {
    static_assert(ConstraintConcept<ExampleConstraint,
                                    puzzle,
                                    ExampleConstraintOptionSizes::primary,
                                    ExampleConstraintOptionSizes::secondary>,
                  "CellConstraint does not satisfy ConstraintConcept");
  };

  constexpr static bool supportsPuzzle() {
    return true;
  }

  constexpr static Option<ExampleConstraintOptionSizes::primary> primaryOption(Index row, Index column, Digit digit) {
    return Option<1>{static_cast<OptionId>(
        IdPacking::packId(row, column, digit - 1, puzzle.rows, puzzle.columns, puzzle.digits.size()))};
  }

  constexpr static Option<ExampleConstraintOptionSizes::secondary>
  secondaryOption(Index row, Index column, Digit digit) {
    constexpr auto total = static_cast<uint32_t>(puzzle.rows * puzzle.columns * puzzle.digits.size());
    return {static_cast<OptionId>(
        (total - 1) - IdPacking::packId(row, column, digit - 1, puzzle.rows, puzzle.columns, puzzle.digits.size()))};
  }
};

TEST_CASE("Base Constraint") {

  constexpr auto puzzleIntrinsics = PuzzleIntrinsics<{2, 2, 2}>();
  constexpr auto constraint = ExampleConstraint<puzzleIntrinsics>();

  CHECK_EQ(constraint.type, ConstraintType::NONE);
  CHECK_EQ(constraint.name, "Name");
  CHECK_EQ(constraint.description, "Description");
  checkConstraintOptions<puzzleIntrinsics, 1, 1>(constraint,
                                                 8,
                                                 std::vector<Option<1>>{{0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}},
                                                 8,
                                                 std::vector<Option<1>>{{7}, {6}, {5}, {4}, {3}, {2}, {1}, {0}});
}
