
#include "constraintTemplated/ConstraintInterface.hpp"

#include <doctest.h>
#include <vector>

template <PuzzleIntrinsics intrinsics>
void checkConstraintOptions(const ConstraintInterface<intrinsics>& constraint,
                            uint32_t expectedPrimaryItemsCoverage,
                            const std::vector<Option>& expectedPrimaryOptions,
                            uint32_t expectedSecondaryItemsCoverage,
                            const std::vector<Option>& expectedSecondaryOptions) {

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
      const auto vector = std::vector(array.begin(), array.end());
      CHECK_EQ(expectedPrimaryOptions, vector);
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
      const auto vector = std::vector(array.begin(), array.end());
      CHECK_EQ(expectedSecondaryOptions, vector);
    }
  }
}