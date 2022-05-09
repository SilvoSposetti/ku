#include "KingsMove.h"

#include <cstdint>

ConstraintType KingsMove::getType() const {
  return ConstraintType::KINGS_MOVE;
}

std::string KingsMove::getName() const {
  return "Kings-Move";
}

std::string KingsMove::getDescription() const {
  return "The same digit cannot appear at a king's move away from itself.";
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
  for (const auto& i : INDICES) {
    for (const auto& j : INDICES) {
      const std::vector<std::pair<int32_t, int32_t>> neighbors = getNeighbors(i, j);
      const Sudo currentDigit = board[i][j];
      for (const auto& indexPair : neighbors) {
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

int32_t KingsMove::getDLXConstraintColumnsAmount() const {
  int32_t amount = MAX_DIGIT * MAX_DIGIT; // There are as many boxes-centers as there are cells
  return amount * MAX_DIGIT; // 9 possible digits for each box-center
}

bool KingsMove::getDLXConstraint(Sudo digit, int32_t i, int32_t j, const int32_t columnId) const {
  // columnId encodes the (cell id, possible digit) pair
  const auto& unpacked = unpackId(columnId, TOTAL_DIGITS, MAX_DIGIT);
  const int32_t cellId = unpacked.first;
  const Sudo possibleDigit = static_cast<Sudo>(unpacked.second + 1);
  bool found = false;
  for (const int32_t neighboringBoxId : getNeighboringBoxIds(i, j)) {
    if (neighboringBoxId == cellId) {
      return possibleDigit == digit;
    }
  }
  return false;
}

std::vector<std::pair<int32_t, int32_t>> KingsMove::getNeighbors(int32_t rowIndex, int32_t columnIndex) {
  std::vector<std::pair<int32_t, int32_t>> result;

  // Go through the 3x3 neighbors
  const std::vector<int32_t> neighboringIndices = {-1, 0, 1};
  for (const auto& i : neighboringIndices) {
    for (const auto& j : neighboringIndices) {
      // Ignore the cell in the center
      if (i != 0 || j != 0) {
        int32_t row = rowIndex + i;
        int32_t column = columnIndex + j;
        // Clamp in order to retrieve only valid neighbor indices
        if (row >= MIN_INDEX && row <= MAX_INDEX && column >= MIN_INDEX && column <= MAX_INDEX) {
          result.emplace_back(std::make_pair(row, column));
        }
      }
    }
  }
  return result;
}

std::vector<int32_t> KingsMove::getNeighboringBoxIds(int32_t rowIndex, int32_t columnIndex) {
  std::vector<int32_t> result;

  // Go through the 3x3 neighbors
  const std::vector<int32_t> neighboringIndices = {-1, 0, 1};
  for (const auto& i : neighboringIndices) {
    for (const auto& j : neighboringIndices) {
      const int32_t row = rowIndex + i;
      const int32_t column = columnIndex + j;
      // Only emplace_back indices that are valid
      if (MIN_INDEX <= row && row <= MAX_INDEX && MIN_INDEX <= column && column <= MAX_INDEX) {
        result.emplace_back(packId(row, column, MAX_DIGIT, MAX_DIGIT));
      }
    }
  }
  return result;
}