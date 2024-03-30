#include "PositiveDiagonalEven.h"

#include "ConstraintUtilities.h"

PositiveDiagonalEven::PositiveDiagonalEven()
    : AbstractConstraint("Positive-Diagonal-Even", "The positive diagonal contains only even digits.") {}

std::string PositiveDiagonalEven::getSvgGroup() const {
  const std::string squigglyLine = SvgUtilities::squigglyLine(0, 1, 1, 0);
  return SvgUtilities::createGroup(getName(), squigglyLine, SvgUtilities::getNoFillStroke(thinnestLine));
}

bool PositiveDiagonalEven::satisfy(const std::vector<std::vector<Sudo::Digit>>& board) const {
  // The board satisfies the constraint if the positive diagonal contains only even digits
  const std::vector<std::pair<int32_t, int32_t>> indexPairs = ConstraintUtilities::getAllPositiveDiagonalIndices();
  for (const std::pair<int32_t, int32_t>& pair : indexPairs) {
    if (static_cast<int>(board[pair.first][pair.second]) % 2 != 0) {
      return false;
    }
  }
  return true;
}

int32_t PositiveDiagonalEven::getItemsAmount() const {
  return Sudo::MAX_DIGIT; // There are 9 cells in the positive diagonal
}

bool PositiveDiagonalEven::computeConstraint(Sudo::Digit digit, int32_t i, int32_t j, const int32_t itemId) const {
  // columnId maps to a cell in the diagonal
  const int32_t row = itemId;
  const int32_t column = Sudo::MAX_INDEX - itemId;
  const bool isEvenDigit = (static_cast<int>(digit)) % 2 == 0;
  return i == row && j == column && isEvenDigit;
}