#pragma once
#include <concepts>
#include <stdexcept>

namespace MathUtilities {

template <std::integral T>
constexpr T modulo(T value, T base) {
  if (base == T(0)) {
    throw std::invalid_argument("Modulo divisor cannot be zero.");
  }
  return ((value %= base) < 0) ? value + base : value;
};

} // namespace MathUtilities
