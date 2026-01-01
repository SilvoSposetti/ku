#include "RandomGenerator.hpp"

RandomGenerator::RandomGenerator(std::optional<int32_t> seed) {
  if (!seed) {
    generator = std::mt19937(std::random_device()());
  } else {
    generator = std::mt19937(*seed);
  }
}

int32_t RandomGenerator::uniformInteger(int32_t min, int32_t max) {
  // max and min are both inclusive
  std::uniform_int_distribution<int32_t> uniformIntDistribution(min, max);
  return uniformIntDistribution(generator);
}

float RandomGenerator::uniformFloat(float min, float max) {
  // min is inclusive, max is not
  std::uniform_real_distribution<float> uniformFloatDistribution(min, max);
  return uniformFloatDistribution(generator);
}
