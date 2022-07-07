#include "KingsMove.h"

#include "AbstractConstraint.h"

#include <set>

KingsMove::KingsMove() {
  createDashMap();
}

ConstraintType KingsMove::getType() const {
  return ConstraintType::KINGS_MOVE;
}

std::string KingsMove::getName() const {
  return "Kings-Move";
}

std::string KingsMove::getDescription() const {
  return "The same digit cannot appear at a king's move away from itself.";
}

bool KingsMove::isColumnSecondary(int32_t columnId) const {
  // All columns are secondary
  return true;
}

std::string KingsMove::getSvgGroup() const {
  std::string lines;
  const double cellSize = 1.0 / static_cast<double>(MAX_DIGIT);
  for (int i = 0; i < MAX_DIGIT; i++) {
    const double position = cellSize * i;
    lines += SvgUtilities::line(0, position, 1, position);
    lines += SvgUtilities::line(position, 0, position, 1);
  }
  return SvgUtilities::createGroup(getName(), lines, SvgUtilities::getNoFillStroke(mediumLine));
}

bool KingsMove::satisfy(const std::vector<std::vector<Sudo>>& board) const {
  for (const int& i : INDICES) {
    for (const int& j : INDICES) {
      const std::vector<std::pair<int32_t, int32_t>> neighbors = getNeighbors(i, j);
      const Sudo currentDigit = board[i][j];
      for (const std::pair<int, int>& indexPair : neighbors) {
        if (board[indexPair.first][indexPair.second] == currentDigit) {
          std::cout << "King's Move Clashing cells: " << i << "," << j << "\t" << indexPair.first << ","
                    << indexPair.second << std::endl;
          return false;
        }
      }
    }
  }
  return true;
}

int32_t KingsMove::getDlxConstraintColumnsAmount() const {
  int32_t amount = dashVector.size();
  return amount * MAX_DIGIT;
}

bool KingsMove::getDlxConstraint(Sudo digit, int32_t i, int32_t j, int32_t columnId) const {
  const auto [dashId, digitIndex] = unpackId(columnId, dashVector.size(), MAX_DIGIT);
  const Sudo possibleDigit = static_cast<Sudo>(digitIndex + 1);
  const bool isSame = possibleDigit == digit;

  const auto [firstPair, secondPair] = dashVector[dashId];

  if ((firstPair.first == i && firstPair.second == j) || (secondPair.first == i && secondPair.second == j)) {
    return isSame;
  }
  return false;
}

std::vector<std::pair<int32_t, int32_t>> KingsMove::getNeighbors(int32_t rowIndex, int32_t columnIndex) {
  std::vector<std::pair<int32_t, int32_t>> result;

  // Go through the 3x3 neighbors
  const std::vector<int32_t> neighboringIndices = {-1, 0, 1};
  for (const int& i : neighboringIndices) {
    for (const int& j : neighboringIndices) {
      // Ignore the cell in the center
      if (i != 0 || j != 0) {
        int32_t row = rowIndex + i;
        int32_t column = columnIndex + j;
        // Clamp in order to retrieve only valid neighbor indices
        if (MIN_INDEX <= row && row <= MAX_INDEX && MIN_INDEX <= column && column <= MAX_INDEX) {
          result.emplace_back(std::make_pair(row, column));
        }
      }
    }
  }
  return result;
}

void KingsMove::createDashMap() {
  const std::vector<std::pair<int32_t, int32_t>> neighbors = {{1, -1}, {1, 0}, {1, 1}, {0, 1}};

  std::set<std::pair<std::pair<int32_t, int32_t>, std::pair<int32_t, int32_t>>> set;

  for (int32_t i = 0; i <= MAX_INDEX; ++i) {
    for (int32_t j = 0; j <= MAX_INDEX; ++j) {
      for (const auto& neighbor : neighbors) {
        int32_t boardIndexI = i + neighbor.first;
        int32_t boardIndexJ = j + neighbor.second;
        if (0 <= boardIndexI && boardIndexI <= MAX_INDEX && 0 <= boardIndexJ && boardIndexJ <= MAX_INDEX) {
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
}