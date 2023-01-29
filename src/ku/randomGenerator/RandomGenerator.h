#pragma once

#include <algorithm>
#include <random>

class RandomGenerator {
public:
  RandomGenerator();

  RandomGenerator(int32_t seed);

  int32_t randomUniform(int32_t min, int32_t max);

  template <typename T> std::vector<T> randomShuffle(const std::vector<T>& input) {
    std::vector<T> output = input;
    std::shuffle(output.begin(), output.end(), generator);
    return output;
  }

private:
  std::mt19937 generator;
};