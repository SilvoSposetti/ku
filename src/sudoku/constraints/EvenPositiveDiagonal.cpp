#include "EvenPositiveDiagonal.h"

ConstraintType EvenPositiveDiagonal::getType() const {
  return ConstraintType::EVEN_POSITIVE_DIAGONAL;
}

std::string EvenPositiveDiagonal::getName() const {
  return "Even-Positive-Diagonal";
}

std::string EvenPositiveDiagonal::getDescription() const {
  return "The positive diagonal contains only even digits.";
}

std::string EvenPositiveDiagonal::getSvgGroup() const {
  const std::string line = SvgUtilities::line(0, 1, 1, 0);
  return SvgUtilities::createGroup(getName(), line, SvgUtilities::getNoFillStroke(thinLine));
}

bool EvenPositiveDiagonal::satisfy(const std::vector<std::vector<Sudo>>& board) const {
  // The board satisfies the constraint if the positive diagonal contains only even digits
  const std::vector<std::pair<int32_t, int32_t>> indexPairs = getAllPositiveDiagonalIndices();
  for (const std::pair<int32_t, int32_t>& pair : indexPairs) {
    if (static_cast<int>(board[pair.first][pair.second]) % 2 != 0) {
      return false;
    }
  }
  return true;
}

int32_t EvenPositiveDiagonal::getDlxConstraintColumnsAmount() const {
  return MAX_DIGIT; // There are 9 cells in the main diagonal
}

bool EvenPositiveDiagonal::getDlxConstraint(Sudo digit, int32_t i, int32_t j, const int32_t columnId) const {
  // columnId maps to a cell in the diagonal
  const int32_t row = columnId;
  const int32_t column = MAX_INDEX - columnId;
  const bool isEvenDigit = (static_cast<int>(digit)) % 2 == 0;
  return i == row && j == column && isEvenDigit;
}

std::vector<std::pair<int32_t, int32_t>> EvenPositiveDiagonal::getAllPositiveDiagonalIndices() {
  std::vector<std::pair<int32_t, int32_t>> result;
  for (const int32_t& i : INDICES) {
    result.emplace_back(std::make_pair(i, MAX_INDEX - i));
  }
  return result;
}

bool EvenPositiveDiagonal::isOnPositiveDiagonal(int32_t i, int32_t j) {
  return i + j == MAX_INDEX;
}
