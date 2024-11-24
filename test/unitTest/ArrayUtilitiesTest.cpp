#include "utilities/ArrayUtilities.hpp"

#include <algorithm>
#include <cstdint>
#include <doctest.h>

TEST_CASE("Array Utilities") {

  // TODO: these could be templated test

  SUBCASE("Compile-Time 2D Array creation") {
    SUBCASE("Empty") {
      constexpr auto defaultValue = std::uint8_t(2);
      constexpr auto matrix = ArrayUtilities::create2DArray<uint8_t, 0, 0>(defaultValue);
      CHECK(matrix.empty());
    }

    constexpr auto columns = 5zu;
    constexpr auto rows = 5zu;
    SUBCASE("uint8_t") {
      constexpr auto defaultValue = std::uint8_t(2);
      constexpr auto matrix = ArrayUtilities::create2DArray<uint8_t, columns, rows>(defaultValue);
      CHECK(!matrix.empty());
      CHECK_EQ(matrix.size(), rows);
      for (const auto& row : matrix) {
        CHECK_EQ(row.size(), columns);
        CHECK(std::ranges::all_of(row, [&](const auto& value) { return value == defaultValue; }));
      }
    }

    SUBCASE("int32_t") {
      constexpr auto defaultValue = std::numeric_limits<int32_t>::max();
      constexpr auto matrix = ArrayUtilities::create2DArray<int32_t, columns, rows>(defaultValue);
      CHECK(!matrix.empty());
      CHECK_EQ(matrix.size(), rows);
      for (const auto& row : matrix) {
        CHECK_EQ(row.size(), columns);
        CHECK(std::ranges::all_of(row, [&](const auto& value) { return value == defaultValue; }));
      }
    }
  }

  SUBCASE("Compile-Time Iota Array creation") {

    SUBCASE("Empty") {
      constexpr auto array = ArrayUtilities::createIotaArray<uint8_t, 0>(5);
      CHECK_EQ(array, std::array<uint8_t, 0>());
    }

    constexpr auto amount = 5zu;
    SUBCASE("uint8_t") {
      constexpr auto array = ArrayUtilities::createIotaArray<uint8_t, amount>(5);
      CHECK_EQ(array, std::array<uint8_t, 5>{5, 6, 7, 8, 9});
    }

    SUBCASE("int32_t") {
      constexpr auto array = ArrayUtilities::createIotaArray<int32_t, amount>(-2);
      CHECK_EQ(array, std::array<int32_t, 5>{-2, -1, 0, 1, 2});
    }
  }
}