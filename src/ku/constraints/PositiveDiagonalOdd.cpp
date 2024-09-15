#include "PositiveDiagonalOdd.h"

#include "../drawing/SvgZigZagLine.h"
#include "ConstraintUtilities.h"

PositiveDiagonalOdd::PositiveDiagonalOdd()
    : AbstractConstraint(ConstraintType::POSITIVE_DIAGONAL_ODD,
                         "Positive-Diagonal-Odd",
                         "The positive diagonal contains only odd digits.") {}

std::unique_ptr<SvgGroup> PositiveDiagonalOdd::getSvgGroup(const DrawingOptions& options) const {
  auto group = std::make_unique<SvgGroup>(getName(), std::nullopt, "black", options.thinLine);
  group->add(std::make_unique<SvgZigZagLine>(0, options.size, options.size, 0, options.cellSize / 10.0));
  return group;
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

bool PositiveDiagonalOdd::computeConstraint(Sudo::Digit digit, int32_t i, int32_t j, const int32_t itemId) const {
  // columnId maps to a cell in the diagonal
  const int32_t row = itemId;
  const int32_t column = Sudo::MAX_INDEX - itemId;
  const bool isOddDigit = (static_cast<int>(digit)) % 2 != 0;
  return i == row && j == column && isOddDigit;
}

int32_t PositiveDiagonalOdd::definePrimaryItemsAmount() const {
  return Sudo::MAX_DIGIT; // There are 9 cells in the positive diagonal
}

std::vector<std::vector<int32_t>> PositiveDiagonalOdd::definePrimaryItems() const {
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

int32_t PositiveDiagonalOdd::defineSecondaryItemsAmount() const {
  return 0;
}

std::vector<std::vector<int32_t>> PositiveDiagonalOdd::defineSecondaryItems() const {
  return {};
}
