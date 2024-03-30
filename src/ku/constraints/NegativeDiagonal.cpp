#include "NegativeDiagonal.h"

#include "ConstraintUtilities.h"

NegativeDiagonal::NegativeDiagonal()
    : AbstractConstraint(ConstraintType::NEGATIVE_DIAGONAL,
                         "Negative-Diagonal",
                         "The negative diagonal contains all the digits from " + std::to_string(Sudo::MIN_DIGIT) +
                             " to " + std::to_string(Sudo::MAX_DIGIT) + " exactly once.") {}

std::string NegativeDiagonal::getSvgGroup() const {
  const std::string line = SvgUtilities::line(0, 0, 1, 1);
  return SvgUtilities::createGroup(getName(), line, SvgUtilities::getNoFillStroke(thinLine));
}

bool NegativeDiagonal::satisfy(const std::vector<std::vector<Sudo::Digit>>& board) const {
  // The board satisfies the constraint if the negative diagonal does not contain duplicate digits
  const std::vector<std::pair<int32_t, int32_t>> indexPairs = ConstraintUtilities::getAllNegativeDiagonalIndices();
  for (const auto& digit : Sudo::SUDO_DIGITS) {
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

int32_t NegativeDiagonal::getItemsAmount() const {
  return Sudo::MAX_DIGIT;
}

bool NegativeDiagonal::computeConstraint(Sudo::Digit digit, int32_t i, int32_t j, const int32_t itemId) const {

  // itemId maps directly to a cell on the positive diagonal
  const Sudo::Digit possibleDigit = static_cast<Sudo::Digit>(itemId + 1);
  return ConstraintUtilities::isOnNegativeDiagonal(i, j) && possibleDigit == digit;
}