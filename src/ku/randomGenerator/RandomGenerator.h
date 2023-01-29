#pragma once

#include <algorithm>
#include <random>

class RandomGenerator {
public:
  /** Constructor
   * @param seed The seed for the random number generator. If -1 then a random seed will be used
   */
  RandomGenerator(int32_t seed);

  /** Returns a random integer between min and max (inclusive)
   * @param min Minimum number
   * @param max Maximum number
   * @return A random integer in the range
   */
  int32_t randomUniform(int32_t min, int32_t max);

  /** Returns a copy of the original vector, whose elements are randomly shuffled
   * @param input The original vector
   * @return The shuffled copy
   */
  template <typename T> std::vector<T> randomShuffle(const std::vector<T>& input) {
    std::vector<T> output = input;
    std::shuffle(output.begin(), output.end(), generator);
    return output;
  }

private:
  /// The generator
  std::mt19937 generator;
};