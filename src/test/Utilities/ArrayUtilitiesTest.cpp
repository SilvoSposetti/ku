#include "ArrayUtilities.hpp"

#include <algorithm>
#include <cstdint>
#include <doctest.h>

TEST_SUITE("Array Utilities") {
  constexpr uint32_t columns = 5;
  constexpr uint32_t rows = 7;
  constexpr auto defaultValue = 2;

  TEST_CASE_TEMPLATE_DEFINE("Compile-Time Array (Empty)", T, test_empty) {
    constexpr auto array = ArrayUtilities::createArray<T, 0>(defaultValue);
    CHECK(array.empty());
    CHECK_EQ(array, std::array<T, 0>());
  }
  TEST_CASE_TEMPLATE_INVOKE(test_empty, uint8_t);
  TEST_CASE_TEMPLATE_INVOKE(test_empty, int32_t);
  TEST_CASE_TEMPLATE_INVOKE(test_empty, uint32_t);
  TEST_CASE_TEMPLATE_INVOKE(test_empty, uint64_t);

  TEST_CASE_TEMPLATE_DEFINE("Compile-Time Array", T, test) {
    constexpr auto array = ArrayUtilities::createArray<T, rows>(defaultValue);
    CHECK(!array.empty());
    CHECK_EQ(array.size(), rows);
    CHECK(std::ranges::all_of(array, [&](const auto& element) { return element == defaultValue; }));
  }
  TEST_CASE_TEMPLATE_INVOKE(test, uint8_t);
  TEST_CASE_TEMPLATE_INVOKE(test, int32_t);
  TEST_CASE_TEMPLATE_INVOKE(test, uint32_t);
  TEST_CASE_TEMPLATE_INVOKE(test, uint64_t);

  TEST_CASE_TEMPLATE_DEFINE("Compile-Time 2D Array (Empty)", T, test_2D_empty) {
    constexpr auto matrix = ArrayUtilities::create2DArray<T, 0, 0>(defaultValue);
    CHECK(matrix.empty());
    CHECK_EQ(matrix, std::array<std::array<T, 0>, 0>());
  }
  TEST_CASE_TEMPLATE_INVOKE(test_2D_empty, uint8_t);
  TEST_CASE_TEMPLATE_INVOKE(test_2D_empty, int32_t);
  TEST_CASE_TEMPLATE_INVOKE(test_2D_empty, uint32_t);
  TEST_CASE_TEMPLATE_INVOKE(test_2D_empty, uint64_t);

  TEST_CASE_TEMPLATE_DEFINE("Compile-Time 2D Array", T, test_2D) {
    constexpr auto matrix = ArrayUtilities::create2DArray<T, columns, rows>(defaultValue);
    CHECK(!matrix.empty());
    CHECK_EQ(matrix.size(), rows);
    for (const auto& row : matrix) {
      CHECK_EQ(row.size(), columns);
      CHECK(std::ranges::all_of(row, [&](const auto& value) { return value == defaultValue; }));
    }
  }
  TEST_CASE_TEMPLATE_INVOKE(test_2D, uint8_t);
  TEST_CASE_TEMPLATE_INVOKE(test_2D, int32_t);
  TEST_CASE_TEMPLATE_INVOKE(test_2D, uint32_t);
  TEST_CASE_TEMPLATE_INVOKE(test_2D, uint64_t);

  constexpr auto amount = 3;

  TEST_CASE_TEMPLATE_DEFINE("Compile-Time Iota Array (Empty)", T, test_iota_empty) {
    constexpr auto array = ArrayUtilities::createIotaArray<T, 0>(5);
    CHECK(array.empty());
    CHECK_EQ(array, std::array<T, 0>());
  }
  TEST_CASE_TEMPLATE_INVOKE(test_iota_empty, uint8_t);
  TEST_CASE_TEMPLATE_INVOKE(test_iota_empty, int32_t);
  TEST_CASE_TEMPLATE_INVOKE(test_iota_empty, uint32_t);
  TEST_CASE_TEMPLATE_INVOKE(test_iota_empty, uint64_t);

  TEST_CASE_TEMPLATE_DEFINE("Compile-Time Iota Array", T, test_iota) {
    constexpr auto start = T(2);
    constexpr auto array = ArrayUtilities::createIotaArray<T, amount>(start);
    CHECK_EQ(array.size(), amount);
    CHECK_EQ(array[0], start);
    CHECK_EQ(array[1], start + 1);
    CHECK_EQ(array[2], start + 2);
  }
  TEST_CASE_TEMPLATE_INVOKE(test_iota, uint8_t);
  TEST_CASE_TEMPLATE_INVOKE(test_iota, int32_t);
  TEST_CASE_TEMPLATE_INVOKE(test_iota, uint32_t);
  TEST_CASE_TEMPLATE_INVOKE(test_iota, uint64_t);
}
