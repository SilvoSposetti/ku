#include "utilities/FixedCapacityArray.hpp"

#include <algorithm>
#include <cstdint>
#include <doctest.h>
#include <ranges>
#include <vector>

TEST_SUITE("FixedCapacityArray") {

  TEST_CASE_TEMPLATE_DEFINE("Usage", T, usage) {
    constexpr std::size_t capacity = 5;

    SUBCASE("Default constructor") {
      SUBCASE("Non-Zero Capacity") {
        constexpr FixedCapacityArray<T, capacity> array;
        CHECK(array.empty());
        CHECK(array.size() == 0);
        CHECK(array.capacity() == capacity);
      }

      SUBCASE("Zero Capacity") {
        constexpr FixedCapacityArray<T, 0> array;
        CHECK(array.empty());
        CHECK(array.size() == 0);
        CHECK(array.capacity() == 0);
      }
    }

    SUBCASE("Equality") {
      constexpr auto array = FixedCapacityArray<T, capacity>{0, 1, 2, 3};
      constexpr auto equal = FixedCapacityArray<T, capacity>{0, 1, 2, 3};
      constexpr auto differentSize = FixedCapacityArray<T, capacity>{0, 1, 2};
      constexpr auto differentElements = FixedCapacityArray<T, capacity>{0, 1, 2, 4};
      CHECK_EQ(array, equal);
      CHECK_NE(array, differentSize);
      CHECK_NE(array, differentElements);
    }

    SUBCASE("Copy-Constructor") {
      constexpr auto array = FixedCapacityArray<T, capacity>{0, 1, 2, 3};
      constexpr auto copy = array;
      CHECK_EQ(array, copy);
    }

    SUBCASE("Initializer list") {
      SUBCASE("Regular") {
        constexpr FixedCapacityArray<T, capacity> array{9, 8, 7};
        CHECK(!array.empty());
        CHECK_EQ(array.size(), 3);
        CHECK_EQ(array.capacity(), capacity);
        CHECK_EQ(array[0], 9);
        CHECK_EQ(array[1], 8);
        CHECK_EQ(array[2], 7);
        CHECK_THROWS_AS(array[3], std::out_of_range);
        CHECK_THROWS_AS(array[4], std::out_of_range);
        CHECK_THROWS_AS(array[7], std::out_of_range);
        CHECK_THROWS_AS(array[-1], std::out_of_range);
      }

      SUBCASE("Empty Initializer List") {
        constexpr FixedCapacityArray<T, capacity> array{};
        CHECK(array.empty());
        CHECK_EQ(array.size(), 0);
        CHECK_EQ(array.capacity(), capacity);
        for (std::size_t i = 0; i < array.capacity(); i++) {
          CHECK_THROWS_AS(array[i], std::out_of_range);
        }
      }

      SUBCASE("Too Large Initializer List") {
        const auto tooLargeInitializerList = []() { const FixedCapacityArray<T, capacity> array{9, 8, 7, 6, 5, 4}; };
        CHECK_THROWS_AS(tooLargeInitializerList(), std::out_of_range);
      }
    }

    SUBCASE("Const iterators") {
      constexpr FixedCapacityArray<T, capacity> array{0, 1, 2, 3};
      std::vector<T> elements;
      for (const auto& element : array) { // Can be used in const range-based for loops
        elements.push_back(element + 1);
      }
      CHECK_EQ(elements, std::vector<T>{1, 2, 3, 4});
    }

    SUBCASE("Non-const iterators") {
      auto array = FixedCapacityArray<T, capacity>{3, 5, 1, 4};
      std::ranges::sort(array); // Can be used in non-const functions that modify the internal elements
      constexpr auto expected = FixedCapacityArray<T, capacity>{1, 3, 4, 5};
      CHECK_EQ(array, expected);
    }

    SUBCASE("Accessing index too large for internal size type") {
      if constexpr (!std::is_same_v<T, uint64_t>) {
        constexpr FixedCapacityArray<uint8_t, 10> array;
        constexpr std::size_t index = std::numeric_limits<T>::max() + 1;
        CHECK_THROWS_AS(array[index], std::out_of_range);
      }
    }

    SUBCASE("As span") {

      constexpr FixedCapacityArray<T, capacity> array{2, 3, 4};
      const auto span = array.asSpan();
      CHECK_EQ(span.size(), 3);
      CHECK_EQ(span.size(), array.size());
      CHECK_EQ(span[0], 2);
      CHECK_EQ(span[1], 3);
      CHECK_EQ(span[2], 4);
      CHECK_EQ(span.front(), 2);
      CHECK_EQ(span.back(), 4);
    }

    SUBCASE("Modify elements") {
      auto array = FixedCapacityArray<T, capacity>{0, 1, 2};
      const auto modifyElement = [&](std::size_t index, T value) { array[index] = value; };

      SUBCASE("Inside range") {
        CHECK_EQ(array[0], 0);
        modifyElement(0, 1);
        CHECK_EQ(array[0], 1);

        CHECK_EQ(array[1], 1);
        modifyElement(1, 0);
        CHECK_EQ(array[1], 0);

        CHECK_EQ(array[2], 2);
        modifyElement(2, 5);
        CHECK_EQ(array[2], 5);
      }

      SUBCASE("Outside size") {
        CHECK(array.capacity() > array.size());
        for (std::size_t i = array.size(); i < array.capacity(); i++) {
          CHECK_THROWS_AS(modifyElement(i, 0), std::out_of_range);
        }
      }

      SUBCASE("Outside capacity") {
        for (std::size_t i = array.capacity(); i < array.capacity() + 5; i++)
          CHECK_THROWS_AS(modifyElement(i, 0), std::out_of_range);
      }
    }

    SUBCASE("Push Back") {
      auto array = FixedCapacityArray<T, capacity>{};
      CHECK_EQ(array.size(), 0);
      CHECK_EQ(array.capacity(), capacity);
      CHECK_THROWS_AS(array[0], std::out_of_range);

      CHECK(array.capacity() > 0);
      for (std::size_t i = 0; i < array.capacity(); i++) {
        array.pushBack(0);
        CHECK_EQ(array.size(), i + 1);
        CHECK_EQ(array.capacity(), capacity);
        CHECK_EQ(array[i], 0);
        CHECK_THROWS_AS(array[i + 1], std::out_of_range);
      }

      CHECK_EQ(array.size(), array.capacity());
      CHECK_THROWS_AS(array.pushBack(0), std::out_of_range);
    }
  }

  TEST_CASE_TEMPLATE_INVOKE(usage, uint8_t);
  TEST_CASE_TEMPLATE_INVOKE(usage, uint16_t);
  TEST_CASE_TEMPLATE_INVOKE(usage, uint32_t);
  TEST_CASE_TEMPLATE_INVOKE(usage, uint64_t);
}