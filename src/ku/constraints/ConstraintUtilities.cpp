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

std::vector<std::pair<std::pair<int32_t, int32_t>, std::pair<int32_t, int32_t>>>
createDashVector(std::set<std::pair<int32_t, int32_t>> pattern, bool doTorus) {
  std::set<std::pair<std::pair<int32_t, int32_t>, std::pair<int32_t, int32_t>>> set;
  for (int32_t i = 0; i <= Sudo::MAX_INDEX; ++i) {
    for (int32_t j = 0; j <= Sudo::MAX_INDEX; ++j) {
      for (const auto& [otherI, otherJ] : pattern) {
        int32_t boardIndexI = i + otherI;
        int32_t boardIndexJ = j + otherJ;
        boardIndexI = doTorus ? (boardIndexI + Sudo::MAX_DIGIT) % Sudo::MAX_DIGIT : boardIndexI;
        boardIndexJ = doTorus ? (boardIndexJ + Sudo::MAX_DIGIT) % Sudo::MAX_DIGIT : boardIndexJ;
        if (doTorus || (0 <= boardIndexI && boardIndexI <= Sudo::MAX_INDEX && 0 <= boardIndexJ &&
                        boardIndexJ <= Sudo::MAX_INDEX)) {
          const std::pair<std::pair<int32_t, int32_t>, std::pair<int32_t, int32_t>> element = {
              {i, j}, {boardIndexI, boardIndexJ}};
          const std::pair<std::pair<int32_t, int32_t>, std::pair<int32_t, int32_t>> elementReversed = {element.second,
                                                                                                       element.first};
          if (set.find(element) == set.end() && set.find(elementReversed) == set.end()) {
            set.insert(element);
          }
        }
      }
    }
  }

  std::vector<std::pair<std::pair<int32_t, int32_t>, std::pair<int32_t, int32_t>>> result(set.size());
  int32_t i = 0;
  for (const auto& element : set) {
    result[i] = element;
    i++;
  }
  return result;
}

} // namespace ConstraintUtilities
