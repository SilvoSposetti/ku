#include "NegativeDiagonalOdd.h"

ConstraintType NegativeDiagonalOdd::getType() const {
  return ConstraintType::NEGATIVE_DIAGONAL_ODD;
}

std::string NegativeDiagonalOdd::getName() const {
  return "Negative-Diagonal-Odd";
}

std::string NegativeDiagonalOdd::getDescription() const {
  return "The negative diagonal contains only odd digits.";
}


std::string NegativeDiagonalOdd::getSvgGroup() const {
  const std::string zigZagLine = SvgUtilities::zigZagLine(0, 0, 1, 1);
  return SvgUtilities::createGroup(getName(), zigZagLine, SvgUtilities::getNoFillStroke(thinnestLine));
}

bool NegativeDiagonalOdd::satisfy(const std::vector<std::vector<Sudo>>& board) const {
  // The board satisfies the constraint if the negative diagonal contains only odd digits
  const std::vector<std::pair<int32_t, int32_t>> indexPairs = getAllNegativeDiagonalIndices();
  for (const std::pair<int32_t, int32_t>& pair : indexPairs) {
    if (static_cast<int>(board[pair.first][pair.second]) % 2 == 0) {
      return false;
    }
  }
  return true;
}

int32_t NegativeDiagonalOdd::getDlxConstraintColumnsAmount() const {
  return MAX_DIGIT; // There are 9 cells in the negative diagonal
}

bool NegativeDiagonalOdd::getDlxConstraint(Sudo digit, int32_t i, int32_t j, const int32_t columnId) const {
  // columnId maps to a cell in the diagonal
  const int32_t row = columnId;
  const int32_t column = columnId;
  const bool isOddDigit = (static_cast<int>(digit)) % 2 != 0;
  return i == row && j == column && isOddDigit;
}
