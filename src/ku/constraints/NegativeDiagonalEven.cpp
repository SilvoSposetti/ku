#include "NegativeDiagonalEven.hpp"

#include "../drawing/SvgSquigglyLine.hpp"
#include "ConstraintUtilities.hpp"

NegativeDiagonalEven::NegativeDiagonalEven()
    : AbstractConstraint(ConstraintType::NEGATIVE_DIAGONAL_EVEN,
                         "Negative-Diagonal-Even",
                         "The negative diagonal contains only even digits.") {}

std::unique_ptr<SvgGroup> NegativeDiagonalEven::getSvgGroup(const DrawingOptions& options) const {
  auto group = std::make_unique<SvgGroup>(getName(), std::nullopt, "black", options.thinLine);
  group->add(std::make_unique<SvgSquigglyLine>(0, 0, options.size, options.size, options.cellSize / 10.0));
  return group;
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

bool NegativeDiagonalEven::computeConstraint(Sudo::Digit digit, int32_t i, int32_t j, const int32_t itemId) const {
  // columnId maps to a cell in the diagonal
  const int32_t row = itemId;
  const int32_t column = itemId;
  const bool isEvenDigit = (static_cast<int>(digit)) % 2 == 0;
  return i == row && j == column && isEvenDigit;
}

int32_t NegativeDiagonalEven::definePrimaryItemsAmount() const {
  return Sudo::MAX_DIGIT; // There are 9 cells on the negative diagonal
}

std::vector<std::vector<int32_t>> NegativeDiagonalEven::definePrimaryItems() const {
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

int32_t NegativeDiagonalEven::defineSecondaryItemsAmount() const {
  return 0;
}

std::vector<std::vector<int32_t>> NegativeDiagonalEven::defineSecondaryItems() const {
  return {};
}