#include "utilities/FixedCapacityArray.hpp"

#include <cstdint>
#include <doctest.h>
#include <stdexcept>
#include <vector>

TEST_CASE("FixedCapacityArray") {

  constexpr std::size_t capacity = 5;
  SUBCASE("Default constructor") {
    SUBCASE("Non-Zero Capacity") {
      constexpr FixedCapacityArray<uint32_t, capacity> array;
      CHECK(array.empty());
      CHECK(array.size() == 0);
      CHECK(array.capacity() == capacity);
    }

    SUBCASE("Zero Capacity") {
      constexpr FixedCapacityArray<uint32_t, 0> array;
      CHECK(array.empty());
      CHECK(array.size() == 0);
      CHECK(array.capacity() == 0);
    }
  }

  SUBCASE("Equality") {
    constexpr auto array = FixedCapacityArray<uint32_t, capacity>{0, 1, 2, 3};
    constexpr auto equal = FixedCapacityArray<uint32_t, capacity>{0, 1, 2, 3};
    constexpr auto differentSize = FixedCapacityArray<uint32_t, capacity>{0, 1, 2};
    constexpr auto differentElements = FixedCapacityArray<uint32_t, capacity>{0, 1, 2, 4};
    CHECK_EQ(array, equal);
    CHECK_NE(array, differentSize);
    CHECK_NE(array, differentElements);
  }

  SUBCASE("Copy-Constructor") {
    constexpr auto array = FixedCapacityArray<uint32_t, capacity>{0, 1, 2, 3};
    constexpr auto copy = array;
    CHECK_EQ(array, copy);
  }

  SUBCASE("Initializer list") {
    SUBCASE("Regular") {
      constexpr FixedCapacityArray<uint32_t, capacity> array{9, 8, 7};
      CHECK(!array.empty());
      CHECK_EQ(array.size(), 3);
      CHECK_EQ(array.capacity(), capacity);
      CHECK_EQ(array[0], 9);
      CHECK_EQ(array[1], 8);
      CHECK_EQ(array[2], 7);
      CHECK_THROWS_AS(array[3], std::out_of_range);
      CHECK_THROWS_AS(array[4], std::out_of_range);
    }

    SUBCASE("Empty Initializer List") {
      constexpr FixedCapacityArray<uint32_t, capacity> array{};
      CHECK(array.empty());
      CHECK_EQ(array.size(), 0);
      CHECK_EQ(array.capacity(), capacity);
      for (std::size_t i = 0; i < array.capacity(); i++) {
        CHECK_THROWS_AS(array[i], std::out_of_range);
      }
    }

    SUBCASE("Too Large Initializer List") {
      const auto tooLargeInitializerList = []() {
        const FixedCapacityArray<uint32_t, capacity> array{9, 8, 7, 6, 5, 4};
      };
      CHECK_THROWS_AS(tooLargeInitializerList(), std::out_of_range);
    }
  }

  SUBCASE("Can use range-based for loop") {
    constexpr FixedCapacityArray<uint32_t, capacity> array{0, 1, 2, 3};
    std::vector<uint32_t> elements;
    for (const auto& element : array) {
      elements.push_back(element + 1);
    }
    CHECK_EQ(elements, std::vector<uint32_t>{1, 2, 3, 4});
  }
}