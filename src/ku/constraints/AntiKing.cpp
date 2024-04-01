#include "AntiKing.h"

#include "ConstraintUtilities.h"

AntiKing::AntiKing()
    : AbstractConstraint(
          ConstraintType::ANTI_KING, "Anti-King", "A digit cannot appear at a king's move away from itself.")
    , dashVector(ConstraintUtilities::createDashVector({{1, -1}, {1, 0}, {1, 1}, {0, 1}}, false)) {}

std::string AntiKing::getSvgGroup() const {
  std::string lines;
  const double cellSize = 1.0 / static_cast<double>(Sudo::MAX_DIGIT);
  const double distanceFromCenterAxis = (1 - .618) * cellSize;

  for (const auto& [pairA, pairB] : dashVector) {
    const double abX = cellSize * pairB.first - cellSize * pairA.first;
    const double abY = cellSize * pairB.second - cellSize * pairA.second;
    const double norm = (1. / std::sqrt(abX * abX + abY * abY));
    const double centerAX = cellSize * pairA.first + cellSize * 0.5;
    const double centerAY = cellSize * pairA.second + cellSize * 0.5;
    const double centerBX = centerAX + abX;
    const double centerBY = centerAY + abY;
    const double abNormalizedX = abX * norm;
    const double abNormalizedY = abY * norm;
    const double startX = centerAX + distanceFromCenterAxis * abNormalizedX;
    const double startY = centerAY + distanceFromCenterAxis * abNormalizedY;
    const double endX = centerBX - distanceFromCenterAxis * abNormalizedX;
    const double endY = centerBY - distanceFromCenterAxis * abNormalizedY;

    lines += SvgUtilities::line(startX, startY, endX, endY);
  }
  return SvgUtilities::createGroup(getName(), lines, SvgUtilities::getNoFillStroke(thinnestLine));
}

bool AntiKing::satisfy(const std::vector<std::vector<Sudo::Digit>>& board) const {
  for (const auto& [first, second] : dashVector) {
    if (board[first.first][first.second] == board[second.first][second.second]) {
      return false;
    }
  }
  return true;
}

bool AntiKing::computeConstraint(Sudo::Digit digit, int32_t i, int32_t j, int32_t itemId) const {
  const auto [dashId, digitIndex] = IdPacking::unpackId(itemId, dashVector.size(), Sudo::MAX_DIGIT);
  const Sudo::Digit possibleDigit = static_cast<Sudo::Digit>(digitIndex + 1);
  const bool isSame = possibleDigit == digit;

  const auto [firstPair, secondPair] = dashVector[dashId];

  if ((firstPair.first == i && firstPair.second == j) || (secondPair.first == i && secondPair.second == j)) {
    return isSame;
  }
  return false;
}

int32_t AntiKing::getPrimaryItemsAmount() const {
  return 0;
}

std::vector<std::vector<int32_t>> AntiKing::getPrimaryItems() const {
  return {};
}

int32_t AntiKing::getSecondaryItemsAmount() const {
  return dashVector.size() * Sudo::MAX_DIGIT;
}

std::vector<std::vector<int32_t>> AntiKing::getSecondaryItems() const {
  std::vector<std::vector<int32_t>> secondaryItems;

  for (const auto& [i, j, digit] : optionsOrdered()) {
    std::vector<int32_t> items;
    for (int32_t itemId = 0; itemId < getSecondaryItemsAmount(); itemId++) {
      if (computeConstraint(digit, i, j, itemId)) {
        items.emplace_back(itemId);
      }
    }
    secondaryItems.push_back(items);
  }
  return secondaryItems;
}
