
#include "constraintTemplated/Constraint.hpp"

#include <doctest.h>

template <PuzzleIntrinsics puzzleIntrinsics>
class ExampleConstraint {
public:
  constexpr static ItemsList<puzzleIntrinsics> createPrimaryItems() {
    auto itemsList = ItemsList<puzzleIntrinsics>();
    int32_t counter = 0;
    for (auto& item : itemsList) {
      item.emplace_back(counter);
      counter++;
    }
    return itemsList;
  }

  constexpr static ItemsList<puzzleIntrinsics> createSecondaryItems() {
    auto itemsList = ItemsList<puzzleIntrinsics>();
    int32_t counter = 0;
    for (auto& item : itemsList) {
      item.emplace_back(counter);
      counter += 2;
    }
    return itemsList;
  }
};

TEST_CASE("Base Constraint") {

  constexpr auto puzzleIntrinsics = PuzzleIntrinsics<{2, 2, 2}>();
  // TODO: make this constexpr -> ItemsList should not contain std::vectors
  const auto constraint =
      Constraint<ExampleConstraint<puzzleIntrinsics>, puzzleIntrinsics>(ConstraintType::NONE, "Name", "Description");

  CHECK_EQ(constraint.type, ConstraintType::NONE);
  CHECK_EQ(constraint.name, "Name");
  CHECK_EQ(constraint.description, "Description");

  CHECK_EQ(constraint.getPrimaryItems(), ExampleConstraint<puzzleIntrinsics>::createPrimaryItems());
  CHECK_EQ(constraint.getSecondaryItems(), ExampleConstraint<puzzleIntrinsics>::createSecondaryItems());
}
