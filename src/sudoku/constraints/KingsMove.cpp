#include "KingsMove.h"

#include <functional>

ConstraintType KingsMove::getType() const {
  return ConstraintType::KINGS_MOVE;
}

std::string KingsMove::getName() const {
  return "Kings-Move";
}

std::string KingsMove::getDescription() const {
  return "The same digit cannot appear at a king's move away from itself.";
}

bool KingsMove::hasOptionalConstraints() const {
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

  // // TODO: 6 MOST PROMISING
  int32_t amount = 8 * 8; // 64 4x4 base elements [center, down, right, down-right]
  // amount += 8; // horizontal lines that the base elements don't cover
  // amount += 8; // vertical lines that the base elements don't cover
  return amount * MAX_DIGIT;

  // // TODO: 10
  // int32_t amount = 8 * 8;
  // return amount * MAX_DIGIT;
}

bool KingsMove::getDlxConstraint(Sudo digit, int32_t i, int32_t j, int32_t columnId) const {

  // // TODO: 6 MOST PROMISING
  const int32_t firstGroupMaxColumnId = 8 * 8 * MAX_DIGIT;
  const int32_t secondGroupMaxColumnId = 8 * MAX_DIGIT;
  const int32_t thirdGroupMaxColumnId = 8 * MAX_DIGIT;
  // columnId encodes the elementId or the horizontal/vertical id, possible digit pair
  int32_t id = columnId;
  if (id < firstGroupMaxColumnId) {
    const auto [row, column, digitIndex] = unpackId(id, 8, 8, MAX_DIGIT);
    const Sudo possibleDigit = static_cast<Sudo>(digitIndex + 1);
    const bool isSame = possibleDigit == digit;

    for (const auto& [x, y] : getElementNeighbors(row, column)) {
      if (i == x && j == y) {
        return isSame;
      }
    }
    return false;
  }
  // id -= firstGroupMaxColumnId;
  // if (id < secondGroupMaxColumnId) {
  //   const auto [row, digitIndex] = unpackId(id, 8, MAX_DIGIT);
  //   const Sudo possibleDigit = static_cast<Sudo>(digitIndex + 1);
  //   const bool isSame = possibleDigit == digit;
  //   if ((row == i || row + 1 == i) && j == MAX_INDEX) {
  //     return isSame;
  //   }
  //   return false;
  // }
  // id -= secondGroupMaxColumnId;
  // if (id < thirdGroupMaxColumnId) {
  //   const auto [column, digitIndex] = unpackId(id, 8, MAX_DIGIT);
  //   const Sudo possibleDigit = static_cast<Sudo>(digitIndex + 1);
  //   const bool isSame = possibleDigit == digit;
  //   if ((column == j || column + 1 == j) && i == MAX_INDEX) {
  //     return isSame;
  //   }
  //   return false;
  // }
  // return false;

  // // TODO: 10 (correct?) pair of pairs
  // // columnId encodes the (rowId [0-7], columnId [0-7], possible digit) tuple
  // const auto [row, column, digitIndex] = unpackId(columnId, 8, 8, 9);
  // const Sudo possibleDigit = static_cast<Sudo>(digitIndex + 1);
  // const bool isSame = possibleDigit == digit;

  // for (const auto [firstPair, secondPair] : getElementNeighborPairs(row, column)) {
  //   if (firstPair.first <= MAX_INDEX && firstPair.second <= MAX_INDEX && secondPair.first <= MAX_INDEX &&
  //       secondPair.second <= MAX_INDEX) {
  //     if ((firstPair.first == i && firstPair.second == j) || (secondPair.first == i && secondPair.second == j)) {
  //       return isSame;
  //     }
  //   }
  // }
  // return false;
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

      // int32_t x = row < 0 ? 8 : row % 9;
      // int32_t y = column < 0 ? 8 : column % 9;
      // result.emplace_back(packId(x, y, MAX_DIGIT, MAX_DIGIT));
    }
  }
  return result;
}

std::vector<std::pair<int32_t, int32_t>> KingsMove::getElementNeighbors(int32_t rowIndex, int32_t columnIndex) {
  return {{rowIndex, columnIndex},
          {rowIndex, columnIndex + 1},
          {rowIndex + 1, columnIndex},
          {rowIndex + 1, columnIndex + 1}};
}

std::vector<std::pair<int32_t, int32_t>> KingsMove::getElementNeighborsDynamic(int32_t rowIndex, int32_t columnIndex) {

  const std::vector<std::pair<int32_t, int32_t>> allIndices = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};

  std::vector<std::pair<int32_t, int32_t>> result;

  for (const auto& indices : allIndices) {
    const int32_t row = rowIndex + indices.first;
    const int32_t column = columnIndex + indices.second;

    if (MIN_INDEX <= row && row <= MAX_INDEX && MIN_INDEX <= column && column <= MAX_INDEX) {
      result.emplace_back(std::make_pair(row, column));
    }
  }
  return result;
}

std::vector<std::pair<std::pair<int32_t, int32_t>, std::pair<int32_t, int32_t>>>
KingsMove::getElementNeighborPairs(int32_t rowIndex, int32_t columnIndex) {

  const std::vector<std::pair<std::pair<int32_t, int32_t>, std::pair<int32_t, int32_t>>> allIndices = {
      {{0, 0}, {0, 1}},
      {{0, 0}, {1, 0}},
      {{0, 1}, {1, 0}},
      {{0, 0}, {1, 1}},
  };
  std::vector<std::pair<std::pair<int32_t, int32_t>, std::pair<int32_t, int32_t>>> result = allIndices;
  for (auto& pairOfPairs : result) {
    // auto& firstPair = pairOfPairs.first;
    // auto& secondPair = pairOfPairs.second;
    pairOfPairs.first.first += rowIndex;
    pairOfPairs.first.second += columnIndex;
    pairOfPairs.second.first += rowIndex;
    pairOfPairs.second.second += columnIndex;
  }

  return result;
}