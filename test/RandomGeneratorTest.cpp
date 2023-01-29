#include "randomGenerator/RandomGenerator.h"

#include "Sudoku.h"
#include "doctest.h"

#include <unordered_set>

TEST_CASE("RandomGeneratorTest") {
  const int32_t amount = 1000;

  SUBCASE("Random Uniform") {
    const int32_t minimum = std::numeric_limits<int32_t>::min();
    const int32_t maximum = std::numeric_limits<int32_t>::max();

    SUBCASE("No Seed") {
      RandomGenerator generator1;
      RandomGenerator generator2;

      std::unordered_set<int32_t> set1;
      std::unordered_set<int32_t> set2;
      for (int i = 0; i < amount; i++) {
        set1.insert(generator1.randomUniform(minimum, maximum));
        set2.insert(generator2.randomUniform(minimum, maximum));
      }
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
        set1.insert(generator1.randomUniform(minimum, maximum));
        set2.insert(generator2.randomUniform(minimum, maximum));
      }
      CHECK(set1 != set2);
    }

    SUBCASE("Same Seed") {
      const int32_t seed = 0;
      RandomGenerator generator1(seed);
      RandomGenerator generator2(seed);

      std::unordered_set<int32_t> set1;
      std::unordered_set<int32_t> set2;
      for (int i = 0; i < amount; i++) {
        set1.insert(generator1.randomUniform(minimum, maximum));
        set2.insert(generator2.randomUniform(minimum, maximum));
      }
      CHECK(set1 == set2);
    }
  }

  SUBCASE("Random Shuffle") {
    std::vector<int32_t> vector(amount);
    std::iota(vector.begin(), vector.end(), 0);

    SUBCASE("No Seed") {
      RandomGenerator generator1;
      RandomGenerator generator2;
      const std::vector<int32_t> vector1 = generator1.randomShuffle(vector);
      const std::vector<int32_t> vector2 = generator2.randomShuffle(vector);
      CHECK(vector1 != vector2);
    }

    SUBCASE("Different Seed") {
      const int32_t seed1 = 0;
      const int32_t seed2 = 1;
      RandomGenerator generator1(seed1);
      RandomGenerator generator2(seed2);
      const std::vector<int32_t> vector1 = generator1.randomShuffle(vector);
      const std::vector<int32_t> vector2 = generator2.randomShuffle(vector);
      CHECK(vector1 != vector2);
    }

    SUBCASE("Same Seed") {
      const int32_t seed = 0;
      RandomGenerator generator1(seed);
      RandomGenerator generator2(seed);
      const std::vector<int32_t> vector1 = generator1.randomShuffle(vector);
      const std::vector<int32_t> vector2 = generator2.randomShuffle(vector);
      CHECK(vector1 == vector2);
    }
  }

  // SUBCASE("Random Sudoku") {
  //   std::vector<int32_t> vector(amount);
  //   std::iota(vector.begin(), vector.end(), 0);

  //   const ConstraintType sudokuBaseConstraints = ConstraintType::SUDOKU_CELL | ConstraintType::SUDOKU_ROW |
  //                                                ConstraintType::SUDOKU_COLUMN | ConstraintType::SUDOKU_BOX;

  //   SUBCASE("No Seed") {
  //     Sudoku sudoku1("Sudoku1", sudokuBaseConstraints, 50);
  //     Sudoku sudoku2("Sudoku2", sudokuBaseConstraints, 50);
  //   }

  //   SUBCASE("Different Seed") {
  //     // const int32_t seed1 = 0;
  //     // const int32_t seed2 = 1;
  //     // RandomGenerator generator1(seed1);
  //     // RandomGenerator generator2(seed2);
  //     // const std::vector<int32_t> vector1 = generator1.randomShuffle(vector);
  //     // const std::vector<int32_t> vector2 = generator2.randomShuffle(vector);
  //     // CHECK(vector1 != vector2);
  //   }

  //   SUBCASE("Same Seed") {
  //     // const int32_t seed = 0;
  //     // RandomGenerator generator1(seed);
  //     // RandomGenerator generator2(seed);
  //     // const std::vector<int32_t> vector1 = generator1.randomShuffle(vector);
  //     // const std::vector<int32_t> vector2 = generator2.randomShuffle(vector);
  //     // CHECK(vector1 == vector2);
  //   }
  // }
}
