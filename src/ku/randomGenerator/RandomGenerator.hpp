#pragma once

#include <optional>
#include <random>

class RandomGenerator {
public:
  /** Constructor.
   * @param seed The seed for the random number generator. Uses a random seed if not available.
   */
  RandomGenerator(std::optional<int32_t> seed = std::nullopt);

  /** Returns a random integer in the interval [min, max]
   * @param min Minimum number
   * @param max Maximum number
   * @return A random integer in the range
   */
  int32_t uniformInteger(int32_t min, int32_t max);

  /** Returns a random float in the interval [min, max)
   * @param min Minimum number
   * @param max Maximum number
   * @return A random float in the range
   */
  float uniformFloat(float min, float max);

private:
  /// The generator
  std::mt19937 generator;
};
