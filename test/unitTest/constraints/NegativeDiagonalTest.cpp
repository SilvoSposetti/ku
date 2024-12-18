
#include "constraints/NegativeDiagonal.hpp"

#include <doctest.h>

TEST_SUITE("Constraints: Output") {
  TEST_CASE("Negative-Diagonal") {
    auto constraint = NegativeDiagonal();
    constraint.initialize();
    const std::vector<std::vector<int32_t>> expectedPrimaryItems = {
        {0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
        {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
        {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
        {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
        {},  {},  {0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
        {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
        {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
        {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
        {},  {},  {},  {},  {0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {},  {},  {},  {},  {},  {},  {},  {},  {},
        {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
        {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
        {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
        {},  {},  {},  {},  {},  {},  {0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {},  {},  {},  {},  {},  {},  {},
        {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
        {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
        {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
        {},  {},  {},  {},  {},  {},  {},  {},  {0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {},  {},  {},  {},  {},
        {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
        {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
        {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
        {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {},  {},  {},
        {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
        {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
        {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
        {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {},
        {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
        {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
        {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
        {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {0}, {1}, {2}, {3}, {4}, {5}, {6}, {7},
        {8}, {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
        {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
        {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},
        {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {},  {0}, {1}, {2}, {3}, {4}, {5},
        {6}, {7}, {8},
    };
    const std::vector<std::vector<int32_t>> expectedSecondaryItems = {};
    CHECK_EQ(expectedPrimaryItems, constraint.getPrimaryItems());
    CHECK_EQ(expectedSecondaryItems, constraint.getSecondaryItems());
  }
}
