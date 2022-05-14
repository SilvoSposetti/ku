#include "OddPositiveDiagonal.h"

ConstraintType OddPositiveDiagonal::getType() const {
  return ConstraintType::ODD_POSITIVE_DIAGONAL;
}

std::string OddPositiveDiagonal::getName() const {
  return "Odd-Positive-Diagonal";
}

std::string OddPositiveDiagonal::getDescription() const {
  return "The positive diagonal contains only odd digits.";
}

std::string OddPositiveDiagonal::getSvgGroup() const {
  const std::string line = SvgUtilities::line(0, 1, 1, 0);
  return SvgUtilities::createGroup(getName(), line, SvgUtilities::getNoFillStroke(thinLine));
}

bool OddPositiveDiagonal::satisfy(const std::vector<std::vector<Sudo>>& board) const {
  // The board satisfies the constraint if the positive diagonal contains only even digits
  const std::vector<std::pair<int32_t, int32_t>> indexPairs = getAllPositiveDiagonalIndices();
  for (const std::pair<int32_t, int32_t>& pair : indexPairs) {
    if (static_cast<int>(board[pair.first][pair.second]) % 2 == 0) {
      return false;
    }
  }
  return true;
}

int32_t OddPositiveDiagonal::getDlxConstraintColumnsAmount() const {
  return MAX_DIGIT; // There are 9 cells in the main diagonal
}

bool OddPositiveDiagonal::getDlxConstraint(Sudo digit, int32_t i, int32_t j, const int32_t columnId) const {
  // columnId maps to a cell in the diagonal
  const int32_t row = columnId;
  const int32_t column = MAX_INDEX - columnId;
  const bool isOddDigit = (static_cast<int>(digit)) % 2 != 0;
  return i == row && j == column && isOddDigit;
}

std::vector<std::pair<int32_t, int32_t>> OddPositiveDiagonal::getAllPositiveDiagonalIndices() {
  std::vector<std::pair<int32_t, int32_t>> result;
  for (const int32_t& i : INDICES) {
    result.emplace_back(std::make_pair(i, MAX_INDEX - i));
  }
  return result;
}

bool OddPositiveDiagonal::isOnPositiveDiagonal(int32_t i, int32_t j) {
  return i + j == MAX_INDEX;
}
