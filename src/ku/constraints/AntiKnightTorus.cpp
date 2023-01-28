#include "AntiKnightTorus.h"

AntiKnightTorus::AntiKnightTorus() {
  pattern = {{2, -1}, {2, 1}, {1, 2}, {-1, 2}};
  dashVector = createDashVector(pattern, true);
}

ConstraintType AntiKnightTorus::getType() const {
  return ConstraintType::ANTI_KNIGHT_TORUS;
}

std::string AntiKnightTorus::getName() const {
  return "Anti-Knight-Torus";
}

std::string AntiKnightTorus::getDescription() const {
  return "The same digit cannot appear at a knight's move away from itself. This restriction also wraps around the "
         "edges of the board.";
}

std::string AntiKnightTorus::getSvgGroup() const {
  std::string lines;
  const double cellSize = 1.0 / static_cast<double>(MAX_DIGIT);
  const double distanceFromCenterAxis = (1 - .618) * cellSize;

  // Create set of all point pairs to draw
  std::set<std::pair<std::pair<int32_t, int32_t>, std::pair<int32_t, int32_t>>> dashSet;

  for (int i = -3; i < MAX_INDEX + 3; i++) {
    for (int j = -3; j < MAX_INDEX + 3; j++) {
      for (const auto& [otherI, otherJ] : pattern) {
        const int32_t indexI = i + otherI;
        const int32_t indexJ = j + otherJ;
        const bool isFirstPointOnBoard = i == clamp(i, MIN_INDEX, MAX_INDEX) && j == clamp(j, MIN_INDEX, MAX_INDEX);
        const bool isSecondPointOnBoard =
            indexI == clamp(indexI, MIN_INDEX, MAX_INDEX) && indexJ == clamp(indexJ, MIN_INDEX, MAX_INDEX);
        if (isFirstPointOnBoard || isSecondPointOnBoard) {
          dashSet.insert({{i, j}, {indexI, indexJ}});
        }
      }
    }
  }

  // Draw all dashes in the set
  for (const auto& [pairA, pairB] : dashSet) {
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

bool AntiKnightTorus::satisfy(const std::vector<std::vector<Sudo>>& board) const {
  for (const auto& [first, second] : dashVector) {
    if (board[first.first][first.second] == board[second.first][second.second]) {
      return false;
    }
  }
  return true;
}

int32_t AntiKnightTorus::getDlxConstraintColumnsAmount() const {
  int32_t amount = dashVector.size();
  return amount * MAX_DIGIT;
}

bool AntiKnightTorus::isColumnPrimary(int32_t columnId) const {
  // All columns are secondary
  return false;
}

bool AntiKnightTorus::getDlxConstraint(Sudo digit, int32_t i, int32_t j, int32_t columnId) const {
  const auto [dashId, digitIndex] = unpackId(columnId, dashVector.size(), MAX_DIGIT);
  const Sudo possibleDigit = static_cast<Sudo>(digitIndex + 1);
  const bool isSame = possibleDigit == digit;

  const auto [firstPair, secondPair] = dashVector[dashId];

  if ((firstPair.first == i && firstPair.second == j) || (secondPair.first == i && secondPair.second == j)) {
    return isSame;
  }
  return false;
}