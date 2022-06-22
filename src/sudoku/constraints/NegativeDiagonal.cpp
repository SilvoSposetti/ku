#include "NegativeDiagonal.h"

ConstraintType NegativeDiagonal::getType() const {
  return ConstraintType::NEGATIVE_DIAGONAL;
}

std::string NegativeDiagonal::getName() const {
  return "Negative-Diagonal";
}

std::string NegativeDiagonal::getDescription() const {
  return "The negative diagonal contains all the digts from " + std::to_string(MIN_DIGIT) + " to " +
         std::to_string(MAX_DIGIT) + " exactly once.";
}

bool NegativeDiagonal::hasOptionalConstraints() const {
  return false;
}

std::string NegativeDiagonal::getSvgGroup() const {
  const std::string line = SvgUtilities::line(0, 0, 1, 1);
  return SvgUtilities::createGroup(getName(), line, SvgUtilities::getNoFillStroke(thinLine));
}

bool NegativeDiagonal::satisfy(const std::vector<std::vector<Sudo>>& board) const {
  // The board satisfies the constraint if the negative diagonal does not contain duplicate digits
  const std::vector<std::pair<int32_t, int32_t>> indexPairs = getAllNegativeDiagonalIndices();
  for (const auto& digit : SUDO_DIGITS) {
    int32_t counter = 0;
    for (const auto& pair : indexPairs) {
      if (board[pair.first][pair.second] == digit) {
        counter++;
      }
    }
    if (counter != 1) {
      return false;
    }
  }
  return true;
}

int32_t NegativeDiagonal::getDlxConstraintColumnsAmount() const {
  return MAX_DIGIT;
}

bool NegativeDiagonal::getDlxConstraint(Sudo digit, int32_t i, int32_t j, const int32_t columnId) const {

  // columnId maps directly to a cell on the positive diagonal
  const Sudo possibleDigit = static_cast<Sudo>(columnId + 1);
  return isOnNegativeDiagonal(i, j) && possibleDigit == digit;
}

std::vector<std::pair<int32_t, int32_t>> NegativeDiagonal::getAllNegativeDiagonalIndices() {
  std::vector<std::pair<int32_t, int32_t>> result;
  for (const auto& i : INDICES) {
    result.emplace_back(std::make_pair(i, i));
  }
  return result;
}

bool NegativeDiagonal::isOnNegativeDiagonal(int32_t i, int32_t j) {
  return i == j;
}