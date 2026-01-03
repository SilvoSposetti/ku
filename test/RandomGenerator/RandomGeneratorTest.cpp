#include "RandomGenerator.hpp"

#include <algorithm>
#include <doctest.h>
#include <unordered_set>

TEST_CASE("Random Generator") {
  constexpr int32_t amount = 100;

  SUBCASE("Uniform integer") {
    constexpr int32_t minimum = -100;
    constexpr int32_t maximum = 618;

    SUBCASE("Random Seed") {
      RandomGenerator generator1;
      RandomGenerator generator2;

      std::unordered_set<int32_t> set1;
      std::unordered_set<int32_t> set2;
      for (int i = 0; i < amount; i++) {
        set1.insert(generator1.uniformInteger(minimum, maximum));
        set2.insert(generator2.uniformInteger(minimum, maximum));
      }
      CHECK(std::all_of(
          set1.begin(), set1.end(), [&](auto element) { return minimum <= element && element <= maximum; }));
      CHECK(std::all_of(
          set2.begin(), set2.end(), [&](auto element) { return minimum <= element && element <= maximum; }));
      CHECK(set1 != set2);
    }

    SUBCASE("Different Seed") {
      const int32_t seed1 = 0;
      const int32_t seed2 = 1;
      RandomGenerator generator1(seed1);
      RandomGenerator generator2(seed2);

      std::unordered_set<int32_t> set1;
      std::unordered_set<int32_t> set2;
      for (int i = 0; i < amount; i++) {
        set1.insert(generator1.uniformInteger(minimum, maximum));
        set2.insert(generator2.uniformInteger(minimum, maximum));
      }
      CHECK(std::all_of(
          set1.begin(), set1.end(), [&](auto element) { return minimum <= element && element <= maximum; }));
      CHECK(std::all_of(
          set2.begin(), set2.end(), [&](auto element) { return minimum <= element && element <= maximum; }));
      CHECK(set1 != set2);
    }

    SUBCASE("Same Seed") {
      const int32_t seed = 0;
      RandomGenerator generator1(seed);
      RandomGenerator generator2(seed);

      std::unordered_set<int32_t> set1;
      std::unordered_set<int32_t> set2;
      for (int i = 0; i < amount; i++) {
        set1.insert(generator1.uniformInteger(minimum, maximum));
        set2.insert(generator2.uniformInteger(minimum, maximum));
      }
      CHECK(std::all_of(
          set1.begin(), set1.end(), [&](auto element) { return minimum <= element && element <= maximum; }));
      CHECK(std::all_of(
          set2.begin(), set2.end(), [&](auto element) { return minimum <= element && element <= maximum; }));
      CHECK_EQ(set1, set2);
    }
  }

  SUBCASE("Uniform float") {
    const float minimum = 0.0f;
    const float maximum = 5.0f;

    SUBCASE("Random Seed") {
      RandomGenerator generator1;
      RandomGenerator generator2;

      std::unordered_set<float> set1;
      std::unordered_set<float> set2;
      for (int i = 0; i < amount; i++) {
        set1.insert(generator1.uniformFloat(minimum, maximum));
        set2.insert(generator2.uniformFloat(minimum, maximum));
      }
      CHECK(std::all_of(
          set1.begin(), set1.end(), [&](auto element) { return minimum <= element && element <= maximum; }));
      CHECK(std::all_of(
          set2.begin(), set2.end(), [&](auto element) { return minimum <= element && element <= maximum; }));
      CHECK(set1 != set2);
    }

    SUBCASE("Different Seed") {
      const int32_t seed1 = 0;
      const int32_t seed2 = 1;
      RandomGenerator generator1(seed1);
      RandomGenerator generator2(seed2);

      std::unordered_set<float> set1;
      std::unordered_set<float> set2;
      for (int i = 0; i < amount; i++) {
        set1.insert(generator1.uniformFloat(minimum, maximum));
        set2.insert(generator2.uniformFloat(minimum, maximum));
      }
      CHECK(std::all_of(
          set1.begin(), set1.end(), [&](auto element) { return minimum <= element && element <= maximum; }));
      CHECK(std::all_of(
          set2.begin(), set2.end(), [&](auto element) { return minimum <= element && element <= maximum; }));
      CHECK(set1 != set2);
    }

    SUBCASE("Same Seed") {
      const int32_t seed = 0;
      RandomGenerator generator1(seed);
      RandomGenerator generator2(seed);

      std::unordered_set<float> set1;
      std::unordered_set<float> set2;
      for (int i = 0; i < amount; i++) {
        set1.insert(generator1.uniformFloat(minimum, maximum));
        set2.insert(generator2.uniformFloat(minimum, maximum));
      }
      CHECK(std::all_of(
          set1.begin(), set1.end(), [&](auto element) { return minimum <= element && element <= maximum; }));
      CHECK(std::all_of(
          set2.begin(), set2.end(), [&](auto element) { return minimum <= element && element <= maximum; }));
      CHECK_EQ(set1, set2);
    }
  }
}
