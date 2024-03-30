#include "PositiveDiagonalOdd.h"

#include "ConstraintUtilities.h"

PositiveDiagonalOdd::PositiveDiagonalOdd()
    : AbstractConstraint(ConstraintType::POSITIVE_DIAGONAL_ODD,
                         "Positive-Diagonal-Odd",
                         "The positive diagonal contains only odd digits.") {}

std::string PositiveDiagonalOdd::getSvgGroup() const {
  const std::string zigZagLine = SvgUtilities::zigZagLine(0, 1, 1, 0);
  return SvgUtilities::createGroup(getName(), zigZagLine, SvgUtilities::getNoFillStroke(thinnestLine));
}

bool PositiveDiagonalOdd::satisfy(const std::vector<std::vector<Sudo::Digit>>& board) const {
  // The board satisfies the constraint if the positive diagonal contains only odd digits
  const std::vector<std::pair<int32_t, int32_t>> indexPairs = ConstraintUtilities::getAllPositiveDiagonalIndices();
  for (const std::pair<int32_t, int32_t>& pair : indexPairs) {
    if (static_cast<int>(board[pair.first][pair.second]) % 2 == 0) {
      return false;
    }
  }
  return true;
}

int32_t PositiveDiagonalOdd::getItemsAmount() const {
  return Sudo::MAX_DIGIT; // There are 9 cells in the positive diagonal
}

bool PositiveDiagonalOdd::computeConstraint(Sudo::Digit digit, int32_t i, int32_t j, const int32_t itemId) const {
  // columnId maps to a cell in the diagonal
  const int32_t row = itemId;
  const int32_t column = Sudo::MAX_INDEX - itemId;
  const bool isOddDigit = (static_cast<int>(digit)) % 2 != 0;
  return i == row && j == column && isOddDigit;
}
