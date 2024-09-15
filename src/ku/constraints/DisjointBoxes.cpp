#include "DisjointBoxes.h"

#include "../drawing/SvgCircle.h"
#include "../utilities/IdPacking.h"

#include <iostream>

DisjointBoxes::DisjointBoxes()
    : AbstractConstraint(
          ConstraintType::DISJOINT_BOXES,
          "Disjoint-Boxes",
          "Cells in the same relative position within all 3x3 boxes contain all the digits from 1 to 9 exactly once.") {
}

std::unique_ptr<SvgGroup> DisjointBoxes::getSvgGroup(const DrawingOptions& options) const {
  auto group = std::make_unique<SvgGroup>(getName(), "transparent", "black", options.thinLine);
  const double circleRadius = options.cellSize / 20.0;
  const double squareOffset = (options.cellSize / 2.0) * 0.7;

  for (int i = 0; i <= Sudo::MAX_INDEX; i++) {
    for (int j = 0; j <= Sudo::MAX_INDEX; j++) {
      const double positionX = options.cellSize * (i + .5) + squareOffset * ((i % 3) - 1);
      const double positionY = options.cellSize * (j + .5) + squareOffset * ((j % 3) - 1);

      group->add(std::make_unique<SvgCircle>(positionX, positionY, circleRadius));
    }
  }

  return group;
}

bool DisjointBoxes::satisfy(const std::vector<std::vector<Sudo::Digit>>& board) const {
  for (const auto& digit : Sudo::SUDO_DIGITS) {
    for (int32_t i = 0; i < 3; i++) {
      for (int32_t j = 0; j < 3; j++) {
        int32_t counter = 0;
        for (int addI = 0; addI < 9; addI += 3) {
          for (int addJ = 0; addJ < 9; addJ += 3) {
            if (board[i + addI][j + addJ] == digit) {
              counter++;
            }
          }
        }
        if (counter != 1) {
          std::cout << static_cast<int>(digit) << std::endl;
          return false;
        }
      }
    }
  }
  return true;
}

bool DisjointBoxes::computeConstraint(Sudo::Digit digit, int32_t i, int32_t j, int32_t itemId) const {
  // itemId encodes the (cell ID within a box, possible digit) pair
  const std::pair<int32_t, int32_t> unpacked = IdPacking::unpackId(itemId, Sudo::MAX_DIGIT, Sudo::MAX_DIGIT);
  const int32_t cellIdWithinBox = unpacked.first;
  const Sudo::Digit possibleDigit = static_cast<Sudo::Digit>(unpacked.second + 1);

  const bool isOnSameBoxRow = i % 3 == cellIdWithinBox / 3;
  const bool isOnSameBoxColumn = j % 3 == cellIdWithinBox % 3;

  return isOnSameBoxRow && isOnSameBoxColumn && possibleDigit == digit;
}

int32_t DisjointBoxes::definePrimaryItemsAmount() const {
  return 9 * Sudo::MAX_DIGIT; // 9(places within each box), 9(possible digits in each box)
}

std::vector<std::vector<int32_t>> DisjointBoxes::definePrimaryItems() const {
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

int32_t DisjointBoxes::defineSecondaryItemsAmount() const {
  return 0;
}

std::vector<std::vector<int32_t>> DisjointBoxes::defineSecondaryItems() const {
  return {};
}