#pragma once

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <math.h>
#include <memory>
#include <random>
#include <set>
#include <vector>

enum class Sudo : int32_t { NONE = 0, A = 1, B = 2, C = 3, D = 4, E = 5, F = 6, G = 7, H = 8, I = 9 };

constexpr int32_t MAX_DIGIT = 9;
constexpr int32_t MIN_DIGIT = 1;
constexpr int32_t TOTAL_DIGITS = MAX_DIGIT * MAX_DIGIT;
constexpr int32_t MAX_INDEX = 8;
constexpr int32_t MID_INDEX = 4;
constexpr int32_t MIN_INDEX = 0;
const std::vector<int32_t> INDICES = {0, 1, 2, 3, 4, 5, 6, 7, 8};
const std::vector<Sudo> SUDO_DIGITS = {Sudo::A, Sudo::B, Sudo::C, Sudo::D, Sudo::E, Sudo::F, Sudo::G, Sudo::H, Sudo::I};

static inline std::vector<std::vector<Sudo>> emptyField() {
  return std::vector<std::vector<Sudo>>(MAX_DIGIT, std::vector<Sudo>(MAX_DIGIT, Sudo::NONE));
}

static inline std::vector<std::vector<bool>> fullGivenMask() {
  return std::vector<std::vector<bool>>(MAX_DIGIT, std::vector<bool>(MAX_DIGIT, true));
}

static inline std::vector<std::vector<bool>> emptyGivenMask() {
  return std::vector<std::vector<bool>>(MAX_DIGIT, std::vector<bool>(MAX_DIGIT, false));
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

static inline int32_t clamp(const int32_t value, const int32_t min, const int32_t max) {
  return std::max(std::min(value, max), min);
}

static inline int32_t packId(int32_t x, int32_t y, int32_t xAmount, int32_t yAmount) {
  return x * yAmount + y;
}

static inline int32_t packId(int32_t x, int32_t y, int32_t z, int32_t xAmount, int32_t yAmount, int32_t zAmount) {
  return x * (yAmount * zAmount) + y * (zAmount) + z;
}

static inline std::pair<int32_t, int32_t> unpackId(int32_t packedId, int32_t xAmount, int32_t yAmount) {
  const int32_t first = packedId / yAmount; // 0 <= x < xAmount
  const int32_t second = packedId % yAmount; // 0 <= y < yAmount
  return std::make_pair(first, second);
}

static inline std::tuple<int32_t, int32_t, int32_t>
unpackId(int32_t packedId, int32_t xAmount, int32_t yAmount, int32_t zAmount) {
  const int32_t first = packedId / (yAmount * zAmount); // 0 <= x < xAmount
  const int32_t second = (packedId % (yAmount * zAmount)) / zAmount; //  0 <= y < yAmount
  const int32_t third = packedId % zAmount; //  0 <= z < zAmount
  return std::make_tuple(first, second, third);
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
    std::string secondsString = std::to_string(seconds - minutes * 60);
    secondsString = seconds < 10 ? "0" + secondsString : secondsString;
    return minutesString + ":" + secondsString;
  }

  void printElapsed(const std::string& message) const {
    const std::string elapsedString = elapsedReadable();
    std::cout << message << " [" << elapsedString << "]" << std::endl;
  }

private:
  typedef std::chrono::high_resolution_clock highResolutionClock;
  typedef std::chrono::duration<double, std::ratio<1>> seconds;
  std::chrono::time_point<highResolutionClock> startTime;
};