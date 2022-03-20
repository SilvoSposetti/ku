#pragma once

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

enum class Sudo : int32_t { NONE = 0, A = 1, B = 2, C = 3, D = 4, E = 5, F = 6, G = 7, H = 8, I = 9 };

constexpr int32_t MAX_DIGIT = 9;
constexpr int32_t MIN_DIGIT = 1;
constexpr int32_t TOTAL_DIGITS = 81;
constexpr int32_t MAX_CELL_INDEX = 80;
constexpr int32_t MAX_INDEX = 8;
constexpr int32_t MID_INDEX = 4;
constexpr int32_t MIN_INDEX = 0;
const std::vector<int32_t> INDICES = {0, 1, 2, 3, 4, 5, 6, 7, 8};
const std::vector<Sudo> SUDO_DIGITS = {Sudo::A, Sudo::B, Sudo::C, Sudo::D, Sudo::E, Sudo::F, Sudo::G, Sudo::H, Sudo::I};

enum class ConstraintType : uint64_t {
  NONE = 0,
  SUDOKU_CELL = 1 << 0, // Activated by default when creating new Sudoku, do not need to specify it
  SUDOKU_ROW = 1 << 1,
  SUDOKU_COLUMN = 1 << 2,
  SUDOKU_BOX = 1 << 3,
  MAIN_DIAGONAL = 1 << 4,
  ANTI_DIAGONAL = 1 << 5,
  KINGS_MOVE = 1 << 6,
};

inline ConstraintType operator|(ConstraintType a, ConstraintType b) {
  return static_cast<ConstraintType>(static_cast<uint64_t>(a) | static_cast<uint64_t>(b));
}

inline ConstraintType operator&(ConstraintType a, ConstraintType b) {
  return static_cast<ConstraintType>(static_cast<uint64_t>(a) & static_cast<uint64_t>(b));
}

static std::vector<std::vector<Sudo>> emptyField() {
  std::vector<std::vector<Sudo>> newField;
  for (int8_t i = 0; i <= MAX_INDEX; i++) {
    std::vector<Sudo> row(MAX_INDEX + 1, Sudo::NONE);
    newField.emplace_back(row);
  }
  return newField;
}

static std::vector<std::vector<bool>> fullGivenMask() {
  std::vector<std::vector<bool>> newMask;
  for (int8_t i = 0; i <= MAX_INDEX; i++) {
    std::vector<bool> row(MAX_INDEX + 1, true);
    newMask.emplace_back(row);
  }
  return newMask;
}

static std::vector<std::vector<bool>> emptyGivenMask() {
  std::vector<std::vector<bool>> newMask;
  for (int8_t i = 0; i <= MAX_INDEX; i++) {
    std::vector<bool> row(MAX_INDEX + 1, false);
    newMask.emplace_back(row);
  }
  return newMask;
}

static inline int32_t getCellId(int32_t rowIndex, int32_t columnIndex) {
  return rowIndex * MAX_DIGIT + columnIndex;
}

static inline std::pair<int32_t, int32_t> getIndices(int32_t cellId) {
  return std::make_pair(cellId / MAX_DIGIT, cellId % MAX_DIGIT);
}

enum class SymmetryType {
  RANDOM,
  ONE_DIAGONAL_MIRROR,
  ONE_DIAGONAL_ROTATION,
  TWO_DIAGONALS_MIRROR,
  TWO_DIAGONALS_ROTATION,
  ONE_AXIS_MIRROR,
  ONE_AXIS_ROTATION,
  TWO_AXES_MIRROR,
  TWO_AXES_ROTATION,
  AMOUNT
};

static inline int32_t randomUniform(const int32_t min = MIN_INDEX, const int32_t max = MAX_INDEX) {
  // max and min are both inclusive
  std::random_device randomDevice;
  std::mt19937 generator(randomDevice());
  std::uniform_int_distribution<int32_t> uniformIntDistribution(min, max);
  return uniformIntDistribution(generator);
}

template <typename T> static inline std::vector<T> randomShuffle(const std::vector<T>& input) {
  std::vector<T> vector = input;
  std::shuffle(vector.begin(), vector.end(), std::mt19937(std::random_device()()));
  return vector;
}

static inline int32_t clamp(const int32_t value, const int32_t min, const int32_t max) {
  return std::max(std::min(value, max), min);
}

class Timer {
public:
  Timer()
      : startTime(highResolutionClock::now()) {}
  void reset() {
    startTime = highResolutionClock::now();
  }
  double elapsed() const {
    return std::chrono::duration_cast<seconds>(highResolutionClock::now() - startTime).count();
  }
  std::string elapsedReadable() const {
    const double seconds = elapsed();
    const double minutes = std::floor(seconds / 60.0);
    std::string minutesString = std::to_string(static_cast<int32_t>(minutes));
    minutesString = minutesString.size() == 1 ? "0" + minutesString : minutesString;
    const std::string secondsString = std::to_string(seconds);

    return minutesString + ":" + secondsString;
  }

  void printElapsed(const std::string& message) const {
    const std::string elapsedString = elapsedReadable();
    std::cout << message << " [ " << elapsedString << " ]" << std::endl;
  }

private:
  typedef std::chrono::high_resolution_clock highResolutionClock;
  typedef std::chrono::duration<double, std::ratio<1>> seconds;
  std::chrono::time_point<highResolutionClock> startTime;
};