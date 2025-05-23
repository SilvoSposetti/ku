
#include "constraintTemplated/ConstraintInterface.hpp"

#include <doctest.h>
#include <unordered_set>
#include <vector>

/** Simple check for the basic members of the constraints.
 */
template <typename Constraint>
void memberChecks() {
  constexpr auto constraint = Constraint();
  CHECK_NE(constraint.type, ConstraintType::NONE);
  CHECK(!constraint.name.empty());
  CHECK(!constraint.description.empty());
}

/** Helper to check a set of options and the amount of items that are advertised by the constraint
 * @param options The options
 * @param itemsAmount The amount of items
 */
void checkOptions(const auto& options, int32_t itemsAmount) {
  if (itemsAmount == 0) {
    // If items are not defined, options should not be defined as well
    CHECK(!options.has_value());
  } else {
    // If any item is defined, all of them must be covered by the options
    auto expected = std::vector<int32_t>(itemsAmount, 0);
    std::ranges::iota(expected, 0);
    const auto expectedSet = std::unordered_set(expected.begin(), expected.end());
    std::vector<int32_t> set;
    for (const auto& option : options.value()) {
      for (const auto& element : option) {
        set.push_back(element);
      }
    }
    CHECK(std::ranges::all_of(set, [&](const auto element) { return expectedSet.contains(element); }));
  }
}

/** Helper to generate SUBCASEs for a constraint and a puzzle space where the options are checked for the advertised
 * coverage
 * @param constraintName The class name of the constraint
 * @param rows The amount of rows
 * @param columns The amount of columns
 * @param digits The amount of digits
 */
#define GENERATE_SUBCASE(constraintName, rows, columns, digits)                                                        \
  SUBCASE(#rows "x" #columns "x" #digits) {                                                                            \
    constexpr auto constraint = constraintName<PuzzleIntrinsics<{rows, columns, digits}>{}>();                         \
    CHECK(constraint.supportsPuzzle());                                                                                \
    SUBCASE("PrimaryItems") {                                                                                          \
      checkOptions(constraint.getPrimaryOptions(), constraint.getPrimaryItemsAmount());                                \
    }                                                                                                                  \
    SUBCASE("Secondary items") {                                                                                       \
      checkOptions(constraint.getSecondaryOptions(), constraint.getSecondaryItemsAmount());                            \
    }                                                                                                                  \
  }

/** Helper to confirm that the constraint reproduces an explicit set of options.
 * @param constraint The constraint
 * @param expectedPrimaryItemsCoverage The expected primary items coverage
 * @param expectedPrimaryOptions The expected primary options
 * @param expectedSecondaryItemsCoverage The expected secondary items coverage
 * @param expectedSecondaryOptions The expected secondary options
 */
template <PuzzleIntrinsics intrinsics, std::size_t N, std::size_t M>
void checkConstraintOptions(const ConstraintInterface<intrinsics>& constraint,
                            uint32_t expectedPrimaryItemsCoverage,
                            const std::vector<Option<N>>& expectedPrimaryOptions,
                            uint32_t expectedSecondaryItemsCoverage,
                            const std::vector<Option<M>>& expectedSecondaryOptions) {

  // Primary items
  SUBCASE("Primary Items") {
    if (expectedPrimaryItemsCoverage == 0) {
      CHECK_EQ(constraint.getPrimaryItemsAmount(), 0);
      CHECK(!constraint.getPrimaryOptions().has_value());
    } else {
      REQUIRE(!expectedPrimaryOptions.empty());
      REQUIRE(constraint.getPrimaryOptions().has_value());
      CHECK_EQ(expectedPrimaryOptions.size(), intrinsics.allPossibilities.size());
      CHECK_EQ(constraint.getPrimaryItemsAmount(), expectedPrimaryItemsCoverage);
      const auto array = constraint.getPrimaryOptions().value();
      REQUIRE_EQ(array.size(), expectedPrimaryOptions.size());
      std::size_t i = 0;
      for (const auto& option : array) {
        const auto& expectedOption = expectedPrimaryOptions[i];
        REQUIRE_EQ(option.size(), expectedOption.size());
        std::size_t j = 0;
        for (const auto& element : option) {
          CHECK_EQ(expectedOption[j], element);
          j++;
        }
        i++;
      }
    }
  }
  // Primary items
  SUBCASE("Secondary Items") {
    if (expectedSecondaryItemsCoverage == 0) {
      CHECK_EQ(constraint.getSecondaryItemsAmount(), 0);
      CHECK(!constraint.getSecondaryOptions().has_value());
    } else {
      REQUIRE(!expectedSecondaryOptions.empty());
      REQUIRE(constraint.getSecondaryOptions().has_value());
      CHECK_EQ(expectedSecondaryOptions.size(), intrinsics.allPossibilities.size());
      CHECK_EQ(constraint.getSecondaryItemsAmount(), expectedSecondaryItemsCoverage);
      const auto array = constraint.getSecondaryOptions().value();
      REQUIRE_EQ(array.size(), expectedSecondaryOptions.size());
      std::size_t i = 0;
      for (const auto& option : array) {
        const auto& expectedOption = expectedSecondaryOptions[i];
        REQUIRE_EQ(option.size(), expectedOption.size());
        std::size_t j = 0;
        for (const auto& element : option) {
          CHECK_EQ(expectedOption[j], element);
          j++;
        }
        i++;
      }
    }
  }
}