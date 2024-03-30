#include "NegativeDiagonalOdd.h"

#include "ConstraintUtilities.h"

NegativeDiagonalOdd::NegativeDiagonalOdd()
    : AbstractConstraint("Negative-Diagonal-Odd", "The negative diagonal contains only odd digits.") {}

std::string NegativeDiagonalOdd::getSvgGroup() const {
  const std::string zigZagLine = SvgUtilities::zigZagLine(0, 0, 1, 1);
  return SvgUtilities::createGroup(getName(), zigZagLine, SvgUtilities::getNoFillStroke(thinnestLine));
}

bool NegativeDiagonalOdd::satisfy(const std::vector<std::vector<Sudo::Digit>>& board) const {
  // The board satisfies the constraint if the negative diagonal contains only odd digits
  const std::vector<std::pair<int32_t, int32_t>> indexPairs = ConstraintUtilities::getAllNegativeDiagonalIndices();
  for (const std::pair<int32_t, int32_t>& pair : indexPairs) {
    if (static_cast<int>(board[pair.first][pair.second]) % 2 == 0) {
      return false;
    }
  }
  return true;
}

int32_t NegativeDiagonalOdd::getItemsAmount() const {
  return Sudo::MAX_DIGIT; // There are 9 cells in the negative diagonal
}

bool NegativeDiagonalOdd::computeConstraint(Sudo::Digit digit, int32_t i, int32_t j, const int32_t itemId) const {
  // columnId maps to a cell in the diagonal
  const int32_t row = itemId;
  const int32_t column = itemId;
  const bool isOddDigit = (static_cast<int>(digit)) % 2 != 0;
  return i == row && j == column && isOddDigit;
}
