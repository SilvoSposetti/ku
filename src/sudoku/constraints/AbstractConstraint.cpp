#include "AbstractConstraint.h"

bool AbstractConstraint::isColumnPrimary(int32_t columnId) const {
  return true;
};

std::vector<std::pair<std::pair<int32_t, int32_t>, std::pair<int32_t, int32_t>>>
AbstractConstraint::createDashVector(std::set<std::pair<int32_t, int32_t>> pattern, bool doTorus) {
  std::set<std::pair<std::pair<int32_t, int32_t>, std::pair<int32_t, int32_t>>> set;
  for (int32_t i = 0; i <= MAX_INDEX; ++i) {
    for (int32_t j = 0; j <= MAX_INDEX; ++j) {
      for (const auto& [otherI, otherJ] : pattern) {
        int32_t boardIndexI = i + otherI;
        int32_t boardIndexJ = j + otherJ;
        boardIndexI = doTorus ? (boardIndexI + MAX_DIGIT) % MAX_DIGIT : boardIndexI;
        boardIndexJ = doTorus ? (boardIndexJ + MAX_DIGIT) % MAX_DIGIT : boardIndexJ;
        if (doTorus || (0 <= boardIndexI && boardIndexI <= MAX_INDEX && 0 <= boardIndexJ && boardIndexJ <= MAX_INDEX)) {
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
  std::cout << "dashvector size:" << result.size() << std::endl;
  return result;
}
