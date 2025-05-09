
#include "constraintTemplated/Constraint.hpp"

#include <doctest.h>

template <PuzzleIntrinsics puzzleIntrinsics>
struct ExampleConstraint {

  constexpr static bool supportsPuzzle() {
    return true;
  }

  constexpr static OptionsList<puzzleIntrinsics> createPrimaryItems() {
    auto optionsList = OptionsList<puzzleIntrinsics>();
    int32_t counter = 0;
    for (auto& item : optionsList) {
      item[0] = counter;
      counter++;
    }
    return optionsList;
  }

  constexpr static OptionsList<puzzleIntrinsics> createSecondaryItems() {
    auto optionsList = OptionsList<puzzleIntrinsics>();
    int32_t counter = 0;
    for (auto& item : optionsList) {
      item[0] = counter;
      counter += 2;
    }
    return optionsList;
  }
};

TEST_CASE("Base Constraint") {

  constexpr auto puzzleIntrinsics = PuzzleIntrinsics<{2, 2, 2}>();
  // TODO: make this constexpr -> OptionsList should not contain std::vectors
  const auto constraint =
      Constraint<ExampleConstraint<puzzleIntrinsics>, puzzleIntrinsics>(ConstraintType::NONE, "Name", "Description");

  CHECK_EQ(constraint.type, ConstraintType::NONE);
  CHECK_EQ(constraint.name, "Name");
  CHECK_EQ(constraint.description, "Description");

  CHECK_EQ(constraint.getPrimaryOptions(), ExampleConstraint<puzzleIntrinsics>::createPrimaryItems());
  CHECK_EQ(constraint.getSecondaryOptions(), ExampleConstraint<puzzleIntrinsics>::createSecondaryItems());
}
