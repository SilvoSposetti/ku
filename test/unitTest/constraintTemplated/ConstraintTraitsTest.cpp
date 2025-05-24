
#include "constraintTemplated/ConstraintTraits.hpp"

#include <doctest.h>

TEST_CASE("Constraint Traits") {

  SUBCASE("Construction and constexpr") {
    constexpr auto traits1 = ConstraintTraits(0, 0);
    CHECK_EQ(traits1.primarySize, 0);
    CHECK_EQ(traits1.secondarySize, 0);

    constexpr auto traits2 = ConstraintTraits(7, 9);
    CHECK_EQ(traits2.primarySize, 7);
    CHECK_EQ(traits2.secondarySize, 9);
  }
}