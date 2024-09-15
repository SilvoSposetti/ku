#include "AntiKingTorus.h"

#include "../drawing/SvgLine.h"
#include "../utilities/IdPacking.h"
#include "ConstraintUtilities.h"

#include <algorithm>
#include <cmath>

AntiKingTorus::AntiKingTorus()
    : AbstractConstraint(ConstraintType::ANTI_KING_TORUS,
                         "Anti-King-Torus",
                         "A digit cannot appear at a king's move away from itself. This restriction also wraps around "
                         "the edges of the board.")
    , pattern({{1, -1}, {1, 0}, {1, 1}, {0, 1}})
    , dashVector(ConstraintUtilities::createDashVector(pattern, true)) {}

std::unique_ptr<SvgGroup> AntiKingTorus::getSvgGroup(const DrawingOptions& options) const {
  auto group = std::make_unique<SvgGroup>(getName(), std::nullopt, "black", options.thinLine);
  const double distanceFromCenterAxis = (1 - .618) * options.cellSize;

  // Create set of all point pairs to draw
  std::set<std::pair<std::pair<int32_t, int32_t>, std::pair<int32_t, int32_t>>> dashSet;

  for (int i = -2; i < Sudo::MAX_INDEX + 2; i++) {
    for (int j = -2; j < Sudo::MAX_INDEX + 2; j++) {
      for (const auto& [otherI, otherJ] : pattern) {
        const int32_t indexI = i + otherI;
        const int32_t indexJ = j + otherJ;
        const bool isFirstPointOnBoard = i == std::clamp(i, Sudo::MIN_INDEX, Sudo::MAX_INDEX) &&
                                         j == std::clamp(j, Sudo::MIN_INDEX, Sudo::MAX_INDEX);
        const bool isSecondPointOnBoard = indexI == std::clamp(indexI, Sudo::MIN_INDEX, Sudo::MAX_INDEX) &&
                                          indexJ == std::clamp(indexJ, Sudo::MIN_INDEX, Sudo::MAX_INDEX);
        if (isFirstPointOnBoard || isSecondPointOnBoard) {
          dashSet.insert({{i, j}, {indexI, indexJ}});
        }
      }
    }
  }

  // Draw all dashes in the set
  for (const auto& [pairA, pairB] : dashSet) {
    const double abX = options.cellSize * pairB.first - options.cellSize * pairA.first;
    const double abY = options.cellSize * pairB.second - options.cellSize * pairA.second;
    const double norm = (1. / std::sqrt(abX * abX + abY * abY));
    const double centerAX = options.cellSize * pairA.first + options.cellSize * 0.5;
    const double centerAY = options.cellSize * pairA.second + options.cellSize * 0.5;
    const double centerBX = centerAX + abX;
    const double centerBY = centerAY + abY;
    const double abNormalizedX = abX * norm;
    const double abNormalizedY = abY * norm;
    const double startX = centerAX + distanceFromCenterAxis * abNormalizedX;
    const double startY = centerAY + distanceFromCenterAxis * abNormalizedY;
    const double endX = centerBX - distanceFromCenterAxis * abNormalizedX;
    const double endY = centerBY - distanceFromCenterAxis * abNormalizedY;

    group->add(std::make_unique<SvgLine>(startX, startY, endX, endY));
  }
  return group;
}

bool AntiKingTorus::satisfy(const std::vector<std::vector<Sudo::Digit>>& board) const {
  for (const auto& [first, second] : dashVector) {
    if (board[first.first][first.second] == board[second.first][second.second]) {
      return false;
    }
  }
  return true;
}

bool AntiKingTorus::computeConstraint(Sudo::Digit digit, int32_t i, int32_t j, int32_t columnId) const {
  const auto [dashId, digitIndex] = IdPacking::unpackId(columnId, dashVector.size(), Sudo::MAX_DIGIT);
  const Sudo::Digit possibleDigit = static_cast<Sudo::Digit>(digitIndex + 1);
  const bool isSame = possibleDigit == digit;

  const auto [firstPair, secondPair] = dashVector[dashId];

  if ((firstPair.first == i && firstPair.second == j) || (secondPair.first == i && secondPair.second == j)) {
    return isSame;
  }
  return false;
}

int32_t AntiKingTorus::definePrimaryItemsAmount() const {
  return 0;
}

std::vector<std::vector<int32_t>> AntiKingTorus::definePrimaryItems() const {
  return {};
}

int32_t AntiKingTorus::defineSecondaryItemsAmount() const {
  return dashVector.size() * Sudo::MAX_DIGIT;
}

std::vector<std::vector<int32_t>> AntiKingTorus::defineSecondaryItems() const {
  std::vector<std::vector<int32_t>> secondaryItems;

  for (const auto& [rowIndex, columnIndex, digit] : optionsOrdered()) {
    std::vector<int32_t> items;
    for (int32_t itemId = 0; itemId < getSecondaryItemsAmount(); itemId++) {
      if (computeConstraint(digit, rowIndex, columnIndex, itemId)) {
        items.emplace_back(itemId);
      }
    }
    secondaryItems.push_back(items);
  }
  return secondaryItems;
}
