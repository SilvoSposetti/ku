#pragma once

#include <cstdint>
#include <vector>

namespace Sudo {

/** The enum used to map the
 */
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

/**
 */
std::vector<std::vector<Sudo::Digit>> emptyField();

std::vector<std::vector<bool>> fullGivenMask();

std::vector<std::vector<bool>> emptyGivenMask();
} // namespace Sudo