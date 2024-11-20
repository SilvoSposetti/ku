#include "randomGenerator/RandomGenerator.h"

#include "Sudoku.h"
#include <doctest.h>

#include <algorithm>
#include <unordered_set>

TEST_CASE("Random Generator") {
  constexpr int32_t amount = 1000;

  SUBCASE("Uniform integer") {
    const int32_t minimum = -100.0f;
    const int32_t maximum = 618.0f;

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
          set1.begin(), set1.end(), [&](int32_t element) { return minimum <= element && element <= maximum; }));
      CHECK(std::all_of(
          set2.begin(), set2.end(), [&](int32_t element) { return minimum <= element && element <= maximum; }));
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
          set1.begin(), set1.end(), [&](int32_t element) { return minimum <= element && element <= maximum; }));
      CHECK(std::all_of(
          set2.begin(), set2.end(), [&](int32_t element) { return minimum <= element && element <= maximum; }));
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
          set1.begin(), set1.end(), [&](int32_t element) { return minimum <= element && element <= maximum; }));
      CHECK(std::all_of(
          set2.begin(), set2.end(), [&](int32_t element) { return minimum <= element && element <= maximum; }));
      CHECK(set1 == set2);
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
          set1.begin(), set1.end(), [&](int32_t element) { return minimum <= element && element <= maximum; }));
      CHECK(std::all_of(
          set2.begin(), set2.end(), [&](int32_t element) { return minimum <= element && element <= maximum; }));
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
          set1.begin(), set1.end(), [&](int32_t element) { return minimum <= element && element <= maximum; }));
      CHECK(std::all_of(
          set2.begin(), set2.end(), [&](int32_t element) { return minimum <= element && element <= maximum; }));
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
          set1.begin(), set1.end(), [&](int32_t element) { return minimum <= element && element <= maximum; }));
      CHECK(std::all_of(
          set2.begin(), set2.end(), [&](int32_t element) { return minimum <= element && element <= maximum; }));
      CHECK(set1 == set2);
    }
  }

  SUBCASE("Sudoku") {
    const ConstraintType sudokuBaseConstraints = ConstraintType::SUDOKU_CELL | ConstraintType::SUDOKU_ROW |
                                                 ConstraintType::SUDOKU_COLUMN | ConstraintType::SUDOKU_BOX;
    const int32_t clues = 40;

    SUBCASE("Random Seed") {
      Sudoku sudoku1("Sudoku1", sudokuBaseConstraints, clues);
      Sudoku sudoku2("Sudoku2", sudokuBaseConstraints, clues);

      CHECK(sudoku1.getSolution() != sudoku2.getSolution());
      CHECK(sudoku1.getGivenMask() != sudoku2.getGivenMask());
      CHECK(sudoku1.getField() != sudoku2.getField());
    }

    SUBCASE("Different Seed") {
      Sudoku sudoku1("Sudoku1", sudokuBaseConstraints, clues, 0);
      Sudoku sudoku2("Sudoku2", sudokuBaseConstraints, clues, 1);

      CHECK(sudoku1.getSolution() != sudoku2.getSolution());
      CHECK(sudoku1.getGivenMask() != sudoku2.getGivenMask());
      CHECK(sudoku1.getField() != sudoku2.getField());
    }

    SUBCASE("Same Seed") {
      Sudoku sudoku1("Sudoku1", sudokuBaseConstraints, clues, 0);
      Sudoku sudoku2("Sudoku2", sudokuBaseConstraints, clues, 0);

      CHECK(sudoku1.getSolution() == sudoku2.getSolution());
      CHECK(sudoku1.getGivenMask() == sudoku2.getGivenMask());
      CHECK(sudoku1.getField() == sudoku2.getField());
    }
  }
}
