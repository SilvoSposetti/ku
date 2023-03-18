#pragma once

#include <algorithm>
#include <atomic>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <math.h>
#include <memory>
#include <random>
#include <set>
#include <vector>

namespace Sudo {

enum class Digit : int32_t {
  NONE = 0,
  A = 1,
  B = 2,
  C = 3,
  D = 4,
  E = 5,
  F = 6,
  G = 7,
  H = 8,
  I = 9,
};

constexpr int32_t MAX_DIGIT = 9;
constexpr int32_t MIN_DIGIT = 1;
constexpr int32_t TOTAL_DIGITS = MAX_DIGIT * MAX_DIGIT;
constexpr int32_t MAX_INDEX = 8;
constexpr int32_t MID_INDEX = 4;
constexpr int32_t MIN_INDEX = 0;
const std::vector<int32_t> INDICES = {0, 1, 2, 3, 4, 5, 6, 7, 8};
const std::vector<Digit> SUDO_DIGITS = {
    Digit::A, Digit::B, Digit::C, Digit::D, Digit::E, Digit::F, Digit::G, Digit::H, Digit::I};

static inline std::vector<std::vector<Sudo::Digit>> emptyField() {
  return std::vector<std::vector<Sudo::Digit>>(Sudo::MAX_DIGIT,
                                               std::vector<Sudo::Digit>(Sudo::MAX_DIGIT, Sudo::Digit::NONE));
}

static inline std::vector<std::vector<bool>> fullGivenMask() {
  return std::vector<std::vector<bool>>(Sudo::MAX_DIGIT, std::vector<bool>(Sudo::MAX_DIGIT, true));
}

static inline std::vector<std::vector<bool>> emptyGivenMask() {
  return std::vector<std::vector<bool>>(Sudo::MAX_DIGIT, std::vector<bool>(Sudo::MAX_DIGIT, false));
}

} // namespace Sudo