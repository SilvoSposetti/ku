#include "RandomGenerator.h"

RandomGenerator::RandomGenerator(int32_t seed) {
  if (seed == -1) {
    generator = std::mt19937(std::random_device()());
  } else {
    generator = std::mt19937(seed);
  }
}

int32_t RandomGenerator::randomUniform(int32_t min, int32_t max) {
  // max and min are both inclusive
  std::uniform_int_distribution<int32_t> uniformIntDistribution(min, max);
  return uniformIntDistribution(generator);
}
