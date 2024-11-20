
#include "constraints/NegativeDiagonalOdd.h"

#include <doctest.h>

TEST_SUITE("Constraints: Output") {
  TEST_CASE("Negative-Diagonal-Odd") {
    auto constraint = NegativeDiagonalOdd();
    constraint.initialize();
    const std::vector<std::vector<int32_t>> expectedPrimaryItems = {
        {0}, {}, {0}, {}, {0}, {}, {0}, {}, {0}, {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {},
        {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {},
        {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {},
        {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {1}, {}, {1}, {}, {1}, {},
        {1}, {}, {1}, {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {},
        {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {},
        {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {},
        {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {2}, {}, {2}, {}, {2}, {}, {2}, {}, {2}, {}, {},  {},
        {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {},
        {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {},
        {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {},
        {},  {}, {},  {}, {},  {}, {3}, {}, {3}, {}, {3}, {}, {3}, {}, {3}, {}, {},  {}, {},  {}, {},  {}, {},  {},
        {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {},
        {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {},
        {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {},
        {4}, {}, {4}, {}, {4}, {}, {4}, {}, {4}, {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {},
        {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {},
        {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {},
        {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {5}, {}, {5}, {}, {5}, {},
        {5}, {}, {5}, {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {},
        {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {},
        {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {},
        {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {6}, {}, {6}, {}, {6}, {}, {6}, {}, {6}, {}, {},  {},
        {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {},
        {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {},
        {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {},
        {},  {}, {},  {}, {},  {}, {7}, {}, {7}, {}, {7}, {}, {7}, {}, {7}, {}, {},  {}, {},  {}, {},  {}, {},  {},
        {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {},
        {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {},
        {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {}, {},  {},
        {8}, {}, {8}, {}, {8}, {}, {8}, {}, {8},
    };
    const std::vector<std::vector<int32_t>> expectedSecondaryItems = {};
    CHECK_EQ(expectedPrimaryItems, constraint.getPrimaryItems());
    CHECK_EQ(expectedSecondaryItems, constraint.getSecondaryItems());
  }
}
