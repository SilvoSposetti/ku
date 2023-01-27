#include "AntiKnight.h"

#include <set>

AntiKnight::AntiKnight() {
  pattern = {{2, -1}, {2, 1}, {1, 2}, {-1, 2}};
  dashVector = createDashVector(pattern, false);
}

ConstraintType AntiKnight::getType() const {
  return ConstraintType::ANTI_KNIGHT;
}

std::string AntiKnight::getName() const {
  return "Anti-Knight";
}

std::string AntiKnight::getDescription() const {
  return "The same digit cannot appear at a knight's move away from itself.";
}

std::string AntiKnight::getSvgGroup() const {
  std::string lines;
  const double cellSize = 1.0 / static_cast<double>(MAX_DIGIT);
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

bool AntiKnight::satisfy(const std::vector<std::vector<Sudo>>& board) const {
  for (const auto& [first, second] : dashVector) {
    if (board[first.first][first.second] == board[second.first][second.second]) {
      return false;
    }
  }
  return true;
}

int32_t AntiKnight::getDlxConstraintColumnsAmount() const {
  int32_t amount = dashVector.size();
  return amount * MAX_DIGIT;
}

bool AntiKnight::isColumnPrimary(int32_t columnId) const {
  // All columns are secondary
  return false;
}

bool AntiKnight::getDlxConstraint(Sudo digit, int32_t i, int32_t j, int32_t columnId) const {
  const auto [dashId, digitIndex] = unpackId(columnId, dashVector.size(), MAX_DIGIT);
  const Sudo possibleDigit = static_cast<Sudo>(digitIndex + 1);
  const bool isSame = possibleDigit == digit;

  const auto [firstPair, secondPair] = dashVector[dashId];

  if ((firstPair.first == i && firstPair.second == j) || (secondPair.first == i && secondPair.second == j)) {
    return isSame;
  }
  return false;
}
