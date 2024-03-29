#include "NegativeDiagonalEven.h"

ConstraintType NegativeDiagonalEven::getType() const {
  return ConstraintType::NEGATIVE_DIAGONAL_EVEN;
}

std::string NegativeDiagonalEven::getDescription() const {
  return "The negative diagonal contains only even digits.";
}

std::string NegativeDiagonalEven::getSvgGroup() const {
  const std::string squigglyLine = SvgUtilities::squigglyLine(0, 0, 1, 1);
  return SvgUtilities::createGroup(getName(), squigglyLine, SvgUtilities::getNoFillStroke(thinnestLine));
}

bool NegativeDiagonalEven::satisfy(const std::vector<std::vector<Sudo::Digit>>& board) const {
  // The board satisfies the constraint if the negative diagonal contains only even digits
  const std::vector<std::pair<int32_t, int32_t>> indexPairs = getAllNegativeDiagonalIndices();
  for (const std::pair<int32_t, int32_t>& pair : indexPairs) {
    if (static_cast<int>(board[pair.first][pair.second]) % 2 != 0) {
      return false;
    }
  }
  return true;
}

int32_t NegativeDiagonalEven::getDlxConstraintColumnsAmount() const {
  return Sudo::MAX_DIGIT; // There are 9 cells on the negative diagonal
}

bool NegativeDiagonalEven::getDlxConstraint(Sudo::Digit digit, int32_t i, int32_t j, const int32_t columnId) const {
  // columnId maps to a cell in the diagonal
  const int32_t row = columnId;
  const int32_t column = columnId;
  const bool isEvenDigit = (static_cast<int>(digit)) % 2 == 0;
  return i == row && j == column && isEvenDigit;
}
