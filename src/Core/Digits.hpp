#pragma once

#include "ArrayUtilities.hpp"

#include <array>
#include <cstdint>

/** The type of all Digits
 */
using Digit = uint8_t;

/** Utilities for creating and managing Digits
 */
namespace Digits {

/* The one digit that's invalid (i.e. not set)
 */
constexpr Digit invalidDigit = std::numeric_limits<Digit>::min();

/** Computes whether a digit is valid
 * @param digit The digit
 * @return Whether the digit is valid
 */
constexpr bool isValid(Digit digit) {
  return digit != invalidDigit;
};

/** Computes whether a digit is even
 * The invalid digit is not even.
 * @param digit The digit
 * @return Whether the digit is even
 */
constexpr bool isEven(Digit digit) {
  if (isValid(digit)) {
    return digit % 2 == 0;
  }
  return false;
}

/** Computes whether a digit is odd
 * The invalid digit is not odd.
 * @param digit The digit
 * @return Whether the digit is odd
 */
constexpr bool isOdd(Digit digit) {
  if (isValid(digit)) {
    return digit % 2 == 1;
  }
  return false;
}

/** Constructs a set of digits. Creating five digits implies those will be [1, 2, 3, 4, 5].
 * @tparam count The amount of digits
 * @return A sorted array of digits
 */
template <std::size_t count>
constexpr std::array<Digit, count> createDigits() {
  return ArrayUtilities::createIotaArray<Digit, count>(1);
}

} // namespace Digits
