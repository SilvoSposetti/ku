
#include "constraintTemplated/ConstraintInterface.hpp"

#include <doctest.h>
#include <vector>

template <PuzzleIntrinsics intrinsics>
void checkConstraintOptions(const ConstraintInterface<intrinsics>& constraint,
                            int32_t expectedPrimaryItemsCoverage,
                            const std::optional<std::vector<Option>>& expectedPrimaryOptions,
                            int32_t expectedSecondaryItemsCoverage,
                            const std::optional<std::vector<Option>>& expectedSecondaryOptions) {

  // Primary items
  SUBCASE("Primary Items") {
    if (expectedPrimaryItemsCoverage == 0) {
      CHECK_EQ(constraint.getPrimaryItemsAmount(), 0);
      CHECK(!constraint.getPrimaryOptions().has_value());
    } else {
      REQUIRE(expectedPrimaryOptions.has_value());
      CHECK_EQ(expectedPrimaryOptions.value().size(), intrinsics.allPossibilities.size());
      CHECK_EQ(constraint.getPrimaryItemsAmount(), expectedPrimaryItemsCoverage);
      REQUIRE(constraint.getPrimaryOptions().has_value());
      const auto array = constraint.getPrimaryOptions().value();
      const auto vector = std::vector(array.begin(), array.end());
      CHECK_EQ(expectedPrimaryOptions.value(), vector);
    }
  }
  // Primary items
  SUBCASE("Secondary Items") {
    if (expectedSecondaryItemsCoverage == 0) {
      CHECK_EQ(constraint.getSecondaryItemsAmount(), 0);
      CHECK(!constraint.getSecondaryOptions().has_value());
    } else {
      REQUIRE(expectedSecondaryOptions.has_value());
      CHECK_EQ(expectedSecondaryOptions.value().size(), intrinsics.allPossibilities.size());
      CHECK_EQ(constraint.getSecondaryItemsAmount(), expectedSecondaryItemsCoverage);
      REQUIRE(constraint.getSecondaryOptions().has_value());
      const auto array = constraint.getSecondaryOptions().value();
      const auto vector = std::vector(array.begin(), array.end());
      CHECK_EQ(expectedSecondaryOptions.value(), vector);
    }
  }
}