#pragma once

#include "ConstraintInterface.hpp"
#include "Option.hpp"

#include <doctest.h>
#include <ranges>
#include <unordered_set>
#include <vector>

/** Simple check for the basic members of the constraints.
 * @tparam Constraint The constraint type
 */
template <typename Constraint>
void memberChecks() {
  const auto constraint = Constraint{};
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
    const auto constraint = constraintName<PuzzleIntrinsics<{rows, columns, digits}>{}>();                             \
    CHECK(constraint.supportsPuzzle());                                                                                \
    SUBCASE("PrimaryItems") {                                                                                          \
      checkOptions(constraint.getPrimaryOptions(), constraint.getPrimaryItemsAmount());                                \
    }                                                                                                                  \
    SUBCASE("Secondary items") {                                                                                       \
      checkOptions(constraint.getSecondaryOptions(), constraint.getSecondaryItemsAmount());                            \
    }                                                                                                                  \
  }

/** Compares actual and expected options together
 * @tparam intrinsics The puzzle intrinsics for which the options were built
 * @tparam maxOptionSize The capacity of the expected Options
 * @param actualItemsAmount The computed items amount
 * @param actualOptions The computed options
 * @param expectedCoverage The expected coverage
 * @param expectedOptions The expected options
 */
template <PuzzleIntrinsics intrinsics, std::size_t maxOptionSize>
void checkExpectedOptions(std::size_t actualItemsAmount,
                          const auto& actualOptions,
                          uint32_t expectedCoverage,
                          const std::vector<Option<maxOptionSize>>& expectedOptions) {
  if (expectedCoverage == 0) {
    CHECK_EQ(actualItemsAmount, 0);
    CHECK(!actualOptions.has_value());
  } else {
    REQUIRE(!expectedOptions.empty());
    REQUIRE(actualOptions.has_value());
    // There is one option given for every possibility in the puzzle
    CHECK_EQ(expectedOptions.size(), intrinsics.allPossibilities().size());
    // The coverage is what is expected
    CHECK_EQ(actualItemsAmount, expectedCoverage);
    // The options are exactly the ones expected
    {
      // The options expected and computed have the same size
      REQUIRE_EQ(actualOptions.value().size(), expectedOptions.size());
      // ALl the options are the same
      for (const auto& [actual, expected] : std::views::zip(actualOptions.value(), expectedOptions)) {
        // The options have the same size
        REQUIRE_EQ(actual.size(), expected.size());
        REQUIRE_LE(actual.size(), expected.capacity());
        // ALl the elements inside the options are the same
        for (const auto& [actualElement, expectedElement] : std::views::zip(actual, expected)) {
          REQUIRE_EQ(actualElement, expectedElement);
        }
      }
    }
  }
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
    checkExpectedOptions<intrinsics, N>(constraint.getPrimaryItemsAmount(),
                                        constraint.getPrimaryOptions(),
                                        expectedPrimaryItemsCoverage,
                                        expectedPrimaryOptions);
    checkOptions(constraint.getPrimaryOptions(), constraint.getPrimaryItemsAmount());
  };

  // Secondary items
  SUBCASE("Secondary Items") {
    checkExpectedOptions<intrinsics, M>(constraint.getSecondaryItemsAmount(),
                                        constraint.getSecondaryOptions(),
                                        expectedSecondaryItemsCoverage,
                                        expectedSecondaryOptions);
    checkOptions(constraint.getSecondaryOptions(), constraint.getSecondaryItemsAmount());
  }
}
