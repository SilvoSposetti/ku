#include "NegativeDiagonalEven.h"

#include "ConstraintUtilities.h"

NegativeDiagonalEven::NegativeDiagonalEven()
    : AbstractConstraint(ConstraintType::NEGATIVE_DIAGONAL_EVEN,
                         "Negative-Diagonal-Even",
                         "The negative diagonal contains only even digits.") {}

std::string NegativeDiagonalEven::getSvgGroup() const {
  const std::string squigglyLine = SvgUtilities::squigglyLine(0, 0, 1, 1);
  return SvgUtilities::createGroup(getName(), squigglyLine, SvgUtilities::getNoFillStroke(thinnestLine));
}

bool NegativeDiagonalEven::satisfy(const std::vector<std::vector<Sudo::Digit>>& board) const {
  // The board satisfies the constraint if the negative diagonal contains only even digits
  const std::vector<std::pair<int32_t, int32_t>> indexPairs = ConstraintUtilities::getAllNegativeDiagonalIndices();
  for (const std::pair<int32_t, int32_t>& pair : indexPairs) {
    if (static_cast<int>(board[pair.first][pair.second]) % 2 != 0) {
      return false;
    }
  }
  return true;
}

int32_t NegativeDiagonalEven::getItemsAmount() const {
  return Sudo::MAX_DIGIT; // There are 9 cells on the negative diagonal
}

bool NegativeDiagonalEven::computeConstraint(Sudo::Digit digit, int32_t i, int32_t j, const int32_t itemId) const {
  // columnId maps to a cell in the diagonal
  const int32_t row = itemId;
  const int32_t column = itemId;
  const bool isEvenDigit = (static_cast<int>(digit)) % 2 == 0;
  return i == row && j == column && isEvenDigit;
}
