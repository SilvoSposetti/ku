#include "AbstractConstraint.h"

std::string AbstractConstraint::getName() const {
  return Constraint::getConstraintNameString(getType());
}

bool AbstractConstraint::isColumnPrimary(int32_t columnId) const {
  return true;
};

std::vector<std::vector<int32_t>> AbstractConstraint::getDlxConstraints() const {
  std::vector<std::vector<int32_t>> result(Sudo::INDICES.size() * Sudo::INDICES.size() * Sudo::SUDO_DIGITS.size());
  const int32_t itemsAmount = getDlxConstraintColumnsAmount();
  int32_t counter = 0;
  for (const auto& rowIndex : Sudo::INDICES) {
    for (const auto& columnIndex : Sudo::INDICES) {
      for (const auto& digit : Sudo::SUDO_DIGITS) {
        for (int32_t itemId = 0; itemId < itemsAmount; itemId++) {
          if (getDlxConstraint(digit, rowIndex, columnIndex, itemId)) {
            result[counter].push_back(itemId);
          }
        }
        counter++;
      }
    }
  }
  return result;
};

std::vector<std::pair<std::pair<int32_t, int32_t>, std::pair<int32_t, int32_t>>>
AbstractConstraint::createDashVector(std::set<std::pair<int32_t, int32_t>> pattern, bool doTorus) {
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

bool AbstractConstraint::isSecondaryColumnsOnly() const {
  bool isSecondaryColumnsOnly = true;
  for (int32_t i = 0; i < getDlxConstraintColumnsAmount(); i++) {
    isSecondaryColumnsOnly &= !isColumnPrimary(i);
    if (!isSecondaryColumnsOnly) {
      return false;
    }
  }
  return isSecondaryColumnsOnly;
}
