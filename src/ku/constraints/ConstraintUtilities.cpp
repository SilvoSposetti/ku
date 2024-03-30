#include "ConstraintUtilities.h"

#include "../Sudo.h"

namespace ConstraintUtilities {
std::vector<std::pair<int32_t, int32_t>> getAllPositiveDiagonalIndices() {
  std::vector<std::pair<int32_t, int32_t>> result;
  for (const auto& i : Sudo::INDICES) {
    result.emplace_back(std::make_pair(i, Sudo::MAX_INDEX - i));
  }
  return result;
}

bool isOnPositiveDiagonal(int32_t i, int32_t j) {
  return i + j == Sudo::MAX_INDEX;
}

std::vector<std::pair<int32_t, int32_t>> getAllNegativeDiagonalIndices() {
  std::vector<std::pair<int32_t, int32_t>> result;
  for (const auto& i : Sudo::INDICES) {
    result.emplace_back(std::make_pair(i, i));
  }
  return result;
}

bool isOnNegativeDiagonal(int32_t i, int32_t j) {
  return i == j;
}
} // namespace ConstraintUtilities
