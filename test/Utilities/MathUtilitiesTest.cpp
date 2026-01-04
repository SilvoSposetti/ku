#include "MathUtilities.hpp"

#include <doctest.h>

TEST_CASE("Math Utilities") {

  SUBCASE("Modulo") {
    // Near zero
    CHECK_EQ(MathUtilities::modulo(-8, 4), 0);
    CHECK_EQ(MathUtilities::modulo(-7, 4), 1);
    CHECK_EQ(MathUtilities::modulo(-6, 4), 2);
    CHECK_EQ(MathUtilities::modulo(-5, 4), 3);
    CHECK_EQ(MathUtilities::modulo(-4, 4), 0);
    CHECK_EQ(MathUtilities::modulo(-3, 4), 1);
    CHECK_EQ(MathUtilities::modulo(-2, 4), 2);
    CHECK_EQ(MathUtilities::modulo(-1, 4), 3);
    CHECK_EQ(MathUtilities::modulo(0, 4), 0);
    CHECK_EQ(MathUtilities::modulo(1, 4), 1);
    CHECK_EQ(MathUtilities::modulo(2, 4), 2);
    CHECK_EQ(MathUtilities::modulo(3, 4), 3);
    CHECK_EQ(MathUtilities::modulo(4, 4), 0);
    CHECK_EQ(MathUtilities::modulo(5, 4), 1);
    CHECK_EQ(MathUtilities::modulo(6, 4), 2);
    CHECK_EQ(MathUtilities::modulo(7, 4), 3);
    CHECK_EQ(MathUtilities::modulo(8, 4), 0);

    // Other cases
    CHECK_EQ(MathUtilities::modulo(-73, 4), 3);
    CHECK_EQ(MathUtilities::modulo(158, 4), 2);

    // Modulo with zero
    CHECK_THROWS_AS(MathUtilities::modulo(1985, 0), std::invalid_argument);

    // Modulo 1
    CHECK_EQ(MathUtilities::modulo(-4, 1), 0);
    CHECK_EQ(MathUtilities::modulo(-3, 1), 0);
    CHECK_EQ(MathUtilities::modulo(-2, 1), 0);
    CHECK_EQ(MathUtilities::modulo(-1, 1), 0);
    CHECK_EQ(MathUtilities::modulo(0, 1), 0);
    CHECK_EQ(MathUtilities::modulo(1, 1), 0);
    CHECK_EQ(MathUtilities::modulo(2, 1), 0);
    CHECK_EQ(MathUtilities::modulo(3, 1), 0);
    CHECK_EQ(MathUtilities::modulo(4, 1), 0);
  };
}
