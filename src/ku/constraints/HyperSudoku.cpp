#include "HyperSudoku.hpp"

#include "../drawing/SvgRect.hpp"
#include "../utilities/IdPacking.hpp"

HyperSudoku::HyperSudoku()
    : AbstractConstraint(ConstraintType::HYPER_SUDOKU,
                         "Hyper-Sudoku",
                         "Four 3x3 boxes contain all the digits from 1 to 9 exactly once.")
    , topLeftCorners({{1, 1}, {5, 1}, {1, 5}, {5, 5}}) {}

std::unique_ptr<SvgGroup> HyperSudoku::getSvgGroup(const DrawingOptions& options) const {
  auto group = std::make_unique<SvgGroup>(getName(), "transparent", "black", options.mediumLine);

  for (const auto& [i, j] : topLeftCorners) {
    const double topLeftX = i * options.cellSize;
    const double topLeftY = j * options.cellSize;

    group->add(std::make_unique<SvgRect>(topLeftX, topLeftY, 3 * options.cellSize, 3 * options.cellSize));
  }
  return group;
}

bool HyperSudoku::satisfy(const std::vector<std::vector<Sudo::Digit>>& board) const {
  // The board satisfies the constraint if all boxes do not contain duplicate digits
  for (const auto& [topLeftI, topLeftJ] : topLeftCorners) {
    for (const auto digit : Sudo::SUDO_DIGITS) {
      int32_t count = 0;
      for (int32_t i = topLeftI; i < topLeftI + 3; i++) {
        for (int32_t j = topLeftJ; j < topLeftJ + 3; j++) {
          if (board[i][j] == digit) {
            count++;
          }
        }
      }
      if (count != 1) {
        return false;
      }
    }
  }
  return true;
}

bool HyperSudoku::isCellInBox(int32_t rowIndex, int32_t columnIndex, int32_t boxId) const {
  const auto& [i, j] = topLeftCorners[boxId];
  return (i <= rowIndex && rowIndex < (i + 3)) && (j <= columnIndex && columnIndex < (j + 3));
}

bool HyperSudoku::computeConstraint(Sudo::Digit digit, int32_t i, int32_t j, int32_t itemId) const {
  // itemId encodes the (box id, possible digit) pair
  const auto [boxId, possibleDigitEncoded] = IdPacking::unpackId(itemId, 4, Sudo::MAX_DIGIT);
  const Sudo::Digit possibleDigit = static_cast<Sudo::Digit>(possibleDigitEncoded + 1);

  return isCellInBox(i, j, boxId) && possibleDigit == digit;
}

int32_t HyperSudoku::definePrimaryItemsAmount() const {
  return 4 * Sudo::MAX_DIGIT; // 4 boxes, 9 possible digits for each box
}

std::vector<std::vector<int32_t>> HyperSudoku::definePrimaryItems() const {
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

int32_t HyperSudoku::defineSecondaryItemsAmount() const {
  return 0;
}

std::vector<std::vector<int32_t>> HyperSudoku::defineSecondaryItems() const {
  return {};
}