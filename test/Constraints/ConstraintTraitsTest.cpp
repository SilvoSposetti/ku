#include "ConstraintTraits.hpp"

#include <doctest.h>

struct ExampleConstraint {};

template <>
struct ConstraintTraits<ExampleConstraint> {
  static constexpr std::size_t primarySize = 7;
  static constexpr std::size_t secondarySize = 13;
};

TEST_CASE("Constraint Traits") {

  SUBCASE("Construction and constexpr") {
    CHECK_EQ(ConstraintTraits<ExampleConstraint>::primarySize, 7);
    CHECK_EQ(ConstraintTraits<ExampleConstraint>::secondarySize, 13);
  }
}
