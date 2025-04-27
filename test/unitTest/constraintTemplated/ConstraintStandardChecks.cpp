
#include "constraintTemplated/CellConstraint.hpp"
#include "constraintTemplated/ColumnConstraint.hpp"
#include "constraintTemplated/RowConstraint.hpp"

#include <algorithm>
#include <doctest.h>

// Define test case template invocations as a macro. This is specifically for "square" PuzzleSpaces
#define INVOKE_SQUARE_TEST_CASES_FOR_CONSTRAINT(ConstraintType)                                                        \
  TEST_CASE_TEMPLATE_INVOKE(checks, ConstraintType<PuzzleIntrinsics<{0, 0, 0}>{}>);                                    \
  TEST_CASE_TEMPLATE_INVOKE(checks, ConstraintType<PuzzleIntrinsics<{1, 1, 1}>{}>);                                    \
  TEST_CASE_TEMPLATE_INVOKE(checks, ConstraintType<PuzzleIntrinsics<{2, 2, 2}>{}>);                                    \
  TEST_CASE_TEMPLATE_INVOKE(checks, ConstraintType<PuzzleIntrinsics<{3, 3, 3}>{}>);                                    \
  TEST_CASE_TEMPLATE_INVOKE(checks, ConstraintType<PuzzleIntrinsics<{4, 4, 4}>{}>);                                    \
  TEST_CASE_TEMPLATE_INVOKE(checks, ConstraintType<PuzzleIntrinsics<{5, 5, 5}>{}>);                                    \
  TEST_CASE_TEMPLATE_INVOKE(checks, ConstraintType<PuzzleIntrinsics<{6, 6, 6}>{}>);                                    \
  TEST_CASE_TEMPLATE_INVOKE(checks, ConstraintType<PuzzleIntrinsics<{7, 7, 7}>{}>);                                    \
  TEST_CASE_TEMPLATE_INVOKE(checks, ConstraintType<PuzzleIntrinsics<{8, 8, 8}>{}>);                                    \
  TEST_CASE_TEMPLATE_INVOKE(checks, ConstraintType<PuzzleIntrinsics<{9, 9, 9}>{}>);                                    \
  TEST_CASE_TEMPLATE_INVOKE(checks, ConstraintType<PuzzleIntrinsics<{10, 10, 10}>{}>);

bool checkItems(const auto& items, int32_t itemsAmount) {
  if (itemsAmount == 0) { // If items are not defined, all options must be empty
    return std::ranges::all_of(items, [&](const auto& option) { return option.empty(); });
  } else { // If any item is defined, all of them must be covered
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
      CHECK(checkItems(constraint.primaryItems, primaryItemsAmount));
    }

    SUBCASE("Secondary items") {
      CHECK(checkItems(constraint.secondaryItems, secondaryItemsAmount));
    }
  }

  // Add further constraints here
  INVOKE_SQUARE_TEST_CASES_FOR_CONSTRAINT(CellConstraint);
  INVOKE_SQUARE_TEST_CASES_FOR_CONSTRAINT(RowConstraint);
  INVOKE_SQUARE_TEST_CASES_FOR_CONSTRAINT(ColumnConstraint);
}
