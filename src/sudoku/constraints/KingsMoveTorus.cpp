#include "KingsMoveTorus.h"

#include <set>

KingsMoveTorus::KingsMoveTorus() {
  createDashVector();
}

ConstraintType KingsMoveTorus::getType() const {
  return ConstraintType::KINGS_MOVE;
}

std::string KingsMoveTorus::getName() const {
  return "Kings-Move-Torus";
}

std::string KingsMoveTorus::getDescription() const {
  return "The same digit cannot appear at a king's move away from itself. This restriction also wraps around the edges "
         "of the board.";
}

std::string KingsMoveTorus::getSvgGroup() const {
  std::string lines;
  const double cellSize = 1.0 / static_cast<double>(MAX_DIGIT);
  const double halfCellSize = cellSize * 0.5;
  const double dashLength = .25 * cellSize;
  const double distanceFromCenterAxis = -.125 * cellSize;

  std::vector<int32_t> enlargedIndices = INDICES;
  enlargedIndices.emplace_back(MAX_INDEX + 1);
  // Horizontal dashes:
  for (const int& i : enlargedIndices) {
    for (const int& j : INDICES) {
      const double startX = cellSize * i + distanceFromCenterAxis;
      const double startY = cellSize * j + halfCellSize;
      const double endX = startX + dashLength;
      const double endY = startY;
      lines += SvgUtilities::line(startX, startY, endX, endY);
    }
  }
  // Vertical dashes:
  for (const int& i : INDICES) {
    for (const int& j : enlargedIndices) {
      const double startX = cellSize * i + halfCellSize;
      const double startY = cellSize * j + distanceFromCenterAxis;
      const double endX = startX;
      const double endY = startY + dashLength;
      lines += SvgUtilities::line(startX, startY, endX, endY);
    }
  }
  // Negative diagonal dashes:
  for (const int& i : enlargedIndices) {
    for (const int& j : enlargedIndices) {
      const double startX = cellSize * i + distanceFromCenterAxis;
      const double startY = cellSize * j + distanceFromCenterAxis;
      const double endX = startX + dashLength;
      const double endY = startY + dashLength;
      lines += SvgUtilities::line(startX, startY, endX, endY);
    }
  }
  // Negative diagonal dashes:
  for (const int& i : enlargedIndices) {
    for (const int& j : enlargedIndices) {
      const double startX = cellSize * i - distanceFromCenterAxis;
      const double startY = cellSize * j + distanceFromCenterAxis;
      const double endX = startX - dashLength;
      const double endY = startY + dashLength;
      lines += SvgUtilities::line(startX, startY, endX, endY);
    }
  }
  return SvgUtilities::createGroup(getName(), lines, SvgUtilities::getNoFillStroke(thinnestLine));
}

bool KingsMoveTorus::satisfy(const std::vector<std::vector<Sudo>>& board) const {
  for (const int& i : INDICES) {
    for (const int& j : INDICES) {
      const std::vector<std::pair<int32_t, int32_t>> neighbors = getNeighborsTorus(i, j);
      const Sudo currentDigit = board[i][j];
      for (const std::pair<int, int>& indexPair : neighbors) {
        if (board[indexPair.first][indexPair.second] == currentDigit) {
          std::cout << "King's Move Torus Clashing cells: " << i << "," << j << "\t" << indexPair.first << ","
                    << indexPair.second << std::endl;
          return false;
        }
      }
    }
  }
  return true;
}

int32_t KingsMoveTorus::getDlxConstraintColumnsAmount() const {
  int32_t amount = dashVector.size();
  return amount * MAX_DIGIT;
}

bool KingsMoveTorus::isColumnPrimary(int32_t columnId) const {
  // All columns are secondary
  return false;
}

bool KingsMoveTorus::getDlxConstraint(Sudo digit, int32_t i, int32_t j, int32_t columnId) const {
  const auto [dashId, digitIndex] = unpackId(columnId, dashVector.size(), MAX_DIGIT);
  const Sudo possibleDigit = static_cast<Sudo>(digitIndex + 1);
  const bool isSame = possibleDigit == digit;

  const auto [firstPair, secondPair] = dashVector[dashId];

  if ((firstPair.first == i && firstPair.second == j) || (secondPair.first == i && secondPair.second == j)) {
    return isSame;
  }
  return false;
}

std::vector<std::pair<int32_t, int32_t>> KingsMoveTorus::getNeighborsTorus(int32_t rowIndex, int32_t columnIndex) {
  std::vector<std::pair<int32_t, int32_t>> result;

  // Go through the 3x3 neighbors
  const std::vector<int32_t> neighboringIndices = {-1, 0, 1};
  for (const int& i : neighboringIndices) {
    for (const int& j : neighboringIndices) {
      // Ignore the cell in the center
      if (i != 0 && j != 0) {
        int32_t row = (rowIndex + i + MAX_DIGIT) % MAX_DIGIT;
        int32_t column = (columnIndex + j + MAX_DIGIT) % MAX_DIGIT;
        // Clamp in order to retrieve only valid neighbor indices
        result.emplace_back(std::make_pair(row, column));
      }
    }
  }
  return result;
}

void KingsMoveTorus::createDashVector() {
  const std::vector<std::pair<int32_t, int32_t>> neighbors = {{1, -1}, {1, 0}, {1, 1}, {0, 1}};

  std::set<std::pair<std::pair<int32_t, int32_t>, std::pair<int32_t, int32_t>>> set;

  for (int32_t i = 0; i <= MAX_INDEX; ++i) {
    for (int32_t j = 0; j <= MAX_INDEX; ++j) {
      for (const auto& neighbor : neighbors) {
        int32_t boardIndexI = (i + neighbor.first + MAX_DIGIT) % MAX_DIGIT;
        int32_t boardIndexJ = (j + neighbor.second + MAX_DIGIT) % MAX_DIGIT;
        const std::pair<int32_t, int32_t> firstPair = std::make_pair(i, j);
        const std::pair<int32_t, int32_t> secondPair = std::make_pair(boardIndexI, boardIndexJ);
        const auto element = std::make_pair(firstPair, secondPair);
        if (set.find(element) == set.end()) {
          set.insert(element);
          dashVector.emplace_back(element);
        }
      }
    }
  }
}