#pragma once

#include <algorithm>
#include <array>
#include <numeric>

/** Utilities to create 1D and 2D constexpr arrays initialized with some value(s)
 */
namespace ArrayUtilities {

/** Helper to construct and initialize a 2D array with some default value
 * @tparam T The type of all elements in the array
 * @tparam N the amount of columns in the array
 * @tparam M the amount of rows in the array
 * @param defaultValue The default value for all elements in the array
 * @return A 2D array whose all elemnts are initialized to the same default value
 */
template <typename T, std::size_t N, std::size_t M>
static constexpr std::array<std::array<T, N>, M> create2DArray(const T& defaultValue) {
  auto matrix = std::array<std::array<T, N>, M>();
  for (auto& row : matrix) {
    std::fill(row.begin(), row.end(), defaultValue);
  }
  return matrix;
}

/** Helper to construct an array filled by the iota function
 * @tparam N the amount of elements required in the array
 * @param start The start for the iota function
 * @return An array with size N filled by the iota function
 */
template <typename T, std::size_t N>
static constexpr std::array<T, N> createIotaArray(const T& start) {
  auto array = std::array<T, N>();
  std::ranges::iota(array, start);
  return array;
}
} // namespace ArrayUtilities