
#include "constraintTemplated/CellConstraint.hpp"
#include "constraintTemplated/ColumnConstraint.hpp"
#include "constraintTemplated/RowConstraint.hpp"

#include <algorithm>
#include <doctest.h>
#include <unordered_set>
#include <vector>

#define INVOKE_TEST_CASES_FOR_CONSTRAINT(ConstraintType)                                                               \
  INVOKE_SQUARE_TEST_CASES_FOR_CONSTRAINT(ConstraintType)                                                              \
  INVOKE_ZERO_TEST_CASES_FOR_CONSTRAINT(ConstraintType)                                                                \
  TEST_CASE_TEMPLATE_INVOKE(members, ConstraintType<PuzzleIntrinsics<{0, 0, 0}>{}>);

// Define test case template invocations as a macro. This is specifically for "square" PuzzleSpaces
#define INVOKE_ZERO_TEST_CASES_FOR_CONSTRAINT(ConstraintType)                                                          \
  TEST_CASE_TEMPLATE_INVOKE(zero, ConstraintType<PuzzleIntrinsics<{0, 0, 0}>{}>);

// Define test case template invocations as a macro. This is specifically for "square" PuzzleSpaces
#define INVOKE_SQUARE_TEST_CASES_FOR_CONSTRAINT(ConstraintType)                                                        \
  TEST_CASE_TEMPLATE_INVOKE(standard, ConstraintType<PuzzleIntrinsics<{1, 1, 1}>{}>);                                  \
  TEST_CASE_TEMPLATE_INVOKE(standard, ConstraintType<PuzzleIntrinsics<{2, 2, 2}>{}>);                                  \
  TEST_CASE_TEMPLATE_INVOKE(standard, ConstraintType<PuzzleIntrinsics<{3, 3, 3}>{}>);                                  \
  TEST_CASE_TEMPLATE_INVOKE(standard, ConstraintType<PuzzleIntrinsics<{4, 4, 4}>{}>);                                  \
  TEST_CASE_TEMPLATE_INVOKE(standard, ConstraintType<PuzzleIntrinsics<{5, 5, 5}>{}>);                                  \
  TEST_CASE_TEMPLATE_INVOKE(standard, ConstraintType<PuzzleIntrinsics<{6, 6, 6}>{}>);                                  \
  TEST_CASE_TEMPLATE_INVOKE(standard, ConstraintType<PuzzleIntrinsics<{7, 7, 7}>{}>);                                  \
  TEST_CASE_TEMPLATE_INVOKE(standard, ConstraintType<PuzzleIntrinsics<{8, 8, 8}>{}>);                                  \
  TEST_CASE_TEMPLATE_INVOKE(standard, ConstraintType<PuzzleIntrinsics<{9, 9, 9}>{}>);                                  \
  TEST_CASE_TEMPLATE_INVOKE(standard, ConstraintType<PuzzleIntrinsics<{10, 10, 10}>{}>);

bool checkOptions(const auto& options, int32_t itemsAmount) {
  if (itemsAmount == 0) { // If items are not defined, options don't exist
    return !options.has_value();
  } else { // If any item is defined, all of them must be covered
    auto expected = std::vector<int32_t>(itemsAmount, 0);
    std::ranges::iota(expected, 0);
    const auto expectedSet = std::unordered_set(expected.begin(), expected.end());
    std::vector<int32_t> set;
    for (const auto& option : options.value()) {
      for (const auto& element : option) {
        set.push_back(element);
      }
    }
    return std::ranges::all_of(set, [&](const auto element) { return expectedSet.contains(element); });
  }
}

TEST_SUITE("Constraints: Checks") {

  TEST_CASE_TEMPLATE_DEFINE("Members", T, members) {
    constexpr auto constraint = T();

    SUBCASE("Constraint information exists") {
      CHECK_NE(constraint.type, ConstraintType::NONE);
      CHECK(!constraint.name.empty());
      CHECK(!constraint.description.empty());
    }
  }

  TEST_CASE_TEMPLATE_DEFINE("Square Checks", T, standard) {
    constexpr auto constraint = T();

    SUBCASE("Primary items") {
      CHECK(checkOptions(constraint.primaryOptions, constraint.getPrimaryItemsAmount()));
    }

    SUBCASE("Secondary items") {
      CHECK(checkOptions(constraint.secondaryOptions, constraint.getSecondaryItemsAmount()));
    }
  }

  TEST_CASE_TEMPLATE_DEFINE("Zero Checks ", T, zero) {
    constexpr auto constraint = T();
    SUBCASE("Primary items") {
      CHECK_EQ(constraint.getPrimaryItemsAmount(), 0);
      CHECK_FALSE(constraint.primaryOptions.has_value());
    }
    SUBCASE("Secondary items") {
      CHECK_EQ(constraint.getSecondaryItemsAmount(), 0);
      CHECK_FALSE(constraint.secondaryOptions.has_value());
    }
  }

  // Add further constraints here
  INVOKE_TEST_CASES_FOR_CONSTRAINT(CellConstraint);
  INVOKE_TEST_CASES_FOR_CONSTRAINT(RowConstraint);
  INVOKE_TEST_CASES_FOR_CONSTRAINT(ColumnConstraint);
}
