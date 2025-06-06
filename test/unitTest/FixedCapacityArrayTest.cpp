#include "utilities/FixedCapacityArray.hpp"

#include <cstdint>
#include <doctest.h>
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

    SUBCASE("Can use range-based for loop") {
      constexpr FixedCapacityArray<T, capacity> array{0, 1, 2, 3};
      std::vector<T> elements;
      for (const auto& element : array) {
        elements.push_back(element + 1);
      }
      CHECK_EQ(elements, std::vector<T>{1, 2, 3, 4});
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

    SUBCASE("From array") {
      SUBCASE("Constexpr array") {
        constexpr auto stdArray = std::array<T, capacity>{0, 1, 3, 4, 7};
        constexpr auto array = FixedCapacityArray<T, capacity>::fromArray(stdArray);
        CHECK_EQ(array[0], 0);
        CHECK_EQ(array[1], 1);
        CHECK_EQ(array[2], 3);
        CHECK_EQ(array[3], 4);
        CHECK_EQ(array[4], 7);
      }

      SUBCASE("Array with modified members") {
        auto stdArray = std::array<T, capacity>();
        stdArray[0] = 1;
        stdArray[3] = 9;
        stdArray[4] = 12;
        const auto array = FixedCapacityArray<T, capacity>::fromArray(stdArray);
        CHECK_EQ(array[0], 1);
        CHECK_EQ(array[1], 0);
        CHECK_EQ(array[2], 0);
        CHECK_EQ(array[3], 9);
        CHECK_EQ(array[4], 12);
      }
    }
  }

  TEST_CASE_TEMPLATE_INVOKE(usage, uint8_t);
  TEST_CASE_TEMPLATE_INVOKE(usage, uint16_t);
  TEST_CASE_TEMPLATE_INVOKE(usage, uint32_t);
  TEST_CASE_TEMPLATE_INVOKE(usage, uint64_t);
}