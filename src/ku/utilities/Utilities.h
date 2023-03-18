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
