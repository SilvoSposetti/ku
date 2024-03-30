#include "PositiveDiagonal.h"

#include "ConstraintUtilities.h"

PositiveDiagonal::PositiveDiagonal()
    : AbstractConstraint(ConstraintType::POSITIVE_DIAGONAL,
                         "Positive-Diagonal",
                         "The positive diagonal contains all the digits from " + std::to_string(Sudo::MIN_DIGIT) +
                             " to " + std::to_string(Sudo::MAX_DIGIT) + " exactly once.") {}

std::string PositiveDiagonal::getSvgGroup() const {
  const std::string line = SvgUtilities::line(0, 1, 1, 0);
  return SvgUtilities::createGroup(getName(), line, SvgUtilities::getNoFillStroke(thinLine));
}

bool PositiveDiagonal::satisfy(const std::vector<std::vector<Sudo::Digit>>& board) const {
  // The board satisfies the constraint if the positive diagonal does not contain duplicate digits
  const std::vector<std::pair<int32_t, int32_t>> indexPairs = ConstraintUtilities::getAllPositiveDiagonalIndices();
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

int32_t PositiveDiagonal::getItemsAmount() const {
  return Sudo::MAX_DIGIT;
}

bool PositiveDiagonal::computeConstraint(Sudo::Digit digit, int32_t i, int32_t j, const int32_t itemId) const {

  // columnId maps directly to a cell on the negative diagonal
  const Sudo::Digit possibleDigit = static_cast<Sudo::Digit>(itemId + 1);
  return ConstraintUtilities::isOnPositiveDiagonal(i, j) && possibleDigit == digit;
}
