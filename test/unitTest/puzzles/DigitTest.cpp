#include "puzzles/Digits.hpp"

#include <algorithm>
#include <doctest.h>
#include <set>

TEST_CASE("Digit") {

  SUBCASE("Invalid digit") {
    constexpr auto invalidDigit = Digits::invalidDigit;
    CHECK_EQ(invalidDigit, 0);
  }

  SUBCASE("Valid digits") {
    constexpr Digit digit0 = Digits::invalidDigit;
    constexpr Digit digit1 = 1;
    constexpr Digit digit2 = 2;
    constexpr Digit digit3 = 3;

    CHECK(!Digits::isValid(digit0));
    CHECK(Digits::isValid(digit1));
    CHECK(Digits::isValid(digit2));
    CHECK(Digits::isValid(digit3));
  }

  SUBCASE("Create Digits") {
    constexpr std::size_t digitsCount = 5;
    constexpr auto digits = Digits::createDigits<digitsCount>();

    // There is the correct amount of digits as requested
    CHECK_EQ(digits.size(), digitsCount);
    // The digits are sorted in the array
    CHECK(std::ranges::is_sorted(digits));

    // There are no duplicates
    auto digitsSet = std::set<Digit>(digits.begin(), digits.end());
    CHECK_EQ(digitsSet.size(), digits.size());

    // All digits are between 1 and the count
    const auto digitIsInValidRange = [&](const auto& digit) -> bool { return 1 <= digit && digit <= digitsCount; };
    CHECK(std::ranges::all_of(digits, digitIsInValidRange));

    // None of the digits is the invalid one
    CHECK(std::ranges::all_of(digits, &Digits::isValid));
  }
}