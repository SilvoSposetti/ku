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

bool PositiveDiagonal::computeConstraint(Sudo::Digit digit, int32_t i, int32_t j, const int32_t itemId) const {

  // columnId maps directly to a cell on the negative diagonal
  const Sudo::Digit possibleDigit = static_cast<Sudo::Digit>(itemId + 1);
  return ConstraintUtilities::isOnPositiveDiagonal(i, j) && possibleDigit == digit;
}

int32_t PositiveDiagonal::getPrimaryItemsAmount() const {
  return Sudo::MAX_DIGIT;
}

std::vector<std::vector<int32_t>> PositiveDiagonal::getPrimaryItems() const {
  std::vector<std::vector<int32_t>> primaryItems;

  for (const auto& [rowIndex, columnIndex, digit] : optionsOrdered()) {
    std::vector<int32_t> items;
    for (int32_t itemId = 0; itemId < getPrimaryItemsAmount(); itemId++) {
      if (computeConstraint(digit, rowIndex, columnIndex, itemId)) {
        items.emplace_back(itemId);
      }
    }
    primaryItems.push_back(items);
  }
  return primaryItems;
}

int32_t PositiveDiagonal::getSecondaryItemsAmount() const {
  return 0;
}

std::vector<std::vector<int32_t>> PositiveDiagonal::getSecondaryItems() const {
  return{};
}