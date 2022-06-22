#include "PositiveDiagonalOdd.h"

ConstraintType PositiveDiagonalOdd::getType() const {
  return ConstraintType::POSITIVE_DIAGONAL_ODD;
}

std::string PositiveDiagonalOdd::getName() const {
  return "Positive-Diagonal-Odd";
}

std::string PositiveDiagonalOdd::getDescription() const {
  return "The positive diagonal contains only odd digits.";
}

bool PositiveDiagonalOdd::hasOptionalConstraints() const {
  return false;
}

std::string PositiveDiagonalOdd::getSvgGroup() const {
  const std::string line = SvgUtilities::line(0, 1, 1, 0);
  return SvgUtilities::createGroup(getName(), line, SvgUtilities::getNoFillStroke(thinLine));
}

bool PositiveDiagonalOdd::satisfy(const std::vector<std::vector<Sudo>>& board) const {
  // The board satisfies the constraint if the positive diagonal contains only odd digits
  const std::vector<std::pair<int32_t, int32_t>> indexPairs = getAllPositiveDiagonalIndices();
  for (const std::pair<int32_t, int32_t>& pair : indexPairs) {
    if (static_cast<int>(board[pair.first][pair.second]) % 2 == 0) {
      return false;
    }
  }
  return true;
}

int32_t PositiveDiagonalOdd::getDlxConstraintColumnsAmount() const {
  return MAX_DIGIT; // There are 9 cells in the positive diagonal
}

bool PositiveDiagonalOdd::getDlxConstraint(Sudo digit, int32_t i, int32_t j, const int32_t columnId) const {
  // columnId maps to a cell in the diagonal
  const int32_t row = columnId;
  const int32_t column = MAX_INDEX - columnId;
  const bool isOddDigit = (static_cast<int>(digit)) % 2 != 0;
  return i == row && j == column && isOddDigit;
}
