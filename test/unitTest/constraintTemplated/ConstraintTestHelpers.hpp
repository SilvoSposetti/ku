
#include "constraintTemplated/ConstraintInterface.hpp"

#include <doctest.h>

template <PuzzleIntrinsics intrinsics>
void checkConstraintItems(const ConstraintInterface<intrinsics>& constraint,
                          int32_t expectedPrimaryItemsCoverage,
                          const std::vector<std::vector<int32_t>>& expectedPrimaryItems,
                          int32_t expectedSecondaryItemsCoverage,
                          const std::vector<std::vector<int32_t>>& expectedSecondaryItems) {

  // Primary items
  SUBCASE("Primary Items") {
    CHECK_EQ(expectedPrimaryItems.size(), intrinsics.allPossibilities.size());
    CHECK_EQ(expectedPrimaryItemsCoverage, constraint.getPrimaryItemsAmount());
    const auto array = constraint.getPrimaryItems();
    const auto vector = std::vector(array.begin(), array.end());
    CHECK_EQ(expectedPrimaryItems, vector);
  }
  // Primary items
  SUBCASE("Secondary Items") {
    CHECK_EQ(expectedSecondaryItems.size(), intrinsics.allPossibilities.size());
    CHECK_EQ(expectedSecondaryItemsCoverage, constraint.getSecondaryItemsAmount());
    const auto array = constraint.getSecondaryItems();
    const auto vector = std::vector(array.begin(), array.end());
    CHECK_EQ(expectedSecondaryItems, vector);
  }
}