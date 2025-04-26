
#include "constraintTemplated/CellConstraint.hpp"
#include "constraintTemplated/ColumnConstraint.hpp"
#include "constraintTemplated/RowConstraint.hpp"

#include <algorithm>
#include <doctest.h>

template <PuzzleIntrinsics intrinsics>
bool checkItems(ItemsList<intrinsics> items, int32_t itemsAmount) {
  if (itemsAmount == 0) { // If items are not defined, all options must be empty
    return std::ranges::all_of(items, [&](const auto& option) { return option.empty(); });
  } else { // If any item is defined, all of them must be covered
    // Expecte
    auto expected = std::vector<int32_t>(itemsAmount, 0);
    std::ranges::iota(expected, 0);
    const auto expectedSet = std::unordered_set(expected.begin(), expected.end());

    std::vector<int32_t> set;
    for (const auto& option : items) {
      for (const auto& element : option) {
        set.push_back(element);
      }
    }
    return std::ranges::all_of(set, [&](const auto element) { return expectedSet.contains(element); });
  }
}

TEST_SUITE("ConstraintTemplated: Standard Checks") {

  constexpr auto puzzleIntrinsics = PuzzleIntrinsics<{9, 9, 9}>();

  TEST_CASE_TEMPLATE_DEFINE("Standard Checks ", T, checks) {
    // TODO: make this constexpr -> ItemsList should not contain std::vectors
    const auto constraint = T();
    const auto primaryItemsAmount = constraint.getPrimaryItemsAmount();
    const auto secondaryItemsAmount = constraint.getSecondaryItemsAmount();

    SUBCASE("Constraint information exists") {
      CHECK(!constraint.name.empty());
      CHECK(!constraint.description.empty());
    }

    SUBCASE("Primary items") {
      CHECK(checkItems<puzzleIntrinsics>(constraint.primaryItems, primaryItemsAmount));
    }

    SUBCASE("Secondary items") {
      CHECK(checkItems<puzzleIntrinsics>(constraint.secondaryItems, secondaryItemsAmount));
    }
  }

  // Invocations
  TEST_CASE_TEMPLATE_INVOKE(checks, CellConstraint<puzzleIntrinsics>);
  TEST_CASE_TEMPLATE_INVOKE(checks, RowConstraint<puzzleIntrinsics>);
  TEST_CASE_TEMPLATE_INVOKE(checks, ColumnConstraint<puzzleIntrinsics>);
}
