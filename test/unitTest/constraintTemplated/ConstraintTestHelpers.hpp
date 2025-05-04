
#include "constraintTemplated/ConstraintInterface.hpp"

#include <doctest.h>

template <PuzzleIntrinsics intrinsics>
void checkConstraintOptions(const ConstraintInterface<intrinsics>& constraint,
                            int32_t expectedPrimaryItemsCoverage,
                            const std::vector<std::vector<int32_t>>& expectedPrimaryOptions,
                            int32_t expectedSecondaryItemsCoverage,
                            const std::vector<std::vector<int32_t>>& expectedSecondaryOptions) {

  // Primary items
  SUBCASE("Primary Items") {
    CHECK_EQ(expectedPrimaryOptions.size(), intrinsics.allPossibilities.size());
    CHECK_EQ(expectedPrimaryItemsCoverage, constraint.getPrimaryItemsAmount());
    const auto array = constraint.getPrimaryOptions();
    const auto vector = std::vector(array.begin(), array.end());
    CHECK_EQ(expectedPrimaryOptions, vector);
  }
  // Primary items
  SUBCASE("Secondary Items") {
    CHECK_EQ(expectedSecondaryOptions.size(), intrinsics.allPossibilities.size());
    CHECK_EQ(expectedSecondaryItemsCoverage, constraint.getSecondaryItemsAmount());
    const auto array = constraint.getSecondaryOptions();
    const auto vector = std::vector(array.begin(), array.end());
    CHECK_EQ(expectedSecondaryOptions, vector);
  }
}