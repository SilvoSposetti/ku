#include "KingsMove.h"

#include <cstdint>
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
  // TODO: 1
  // int32_t amount = TOTAL_DIGITS; // There are as many boxes-centers as there are cells
  // return amount * MAX_DIGIT; // 9 possible digits for each box-center

  // TODO: 2 (densest boxes)
  int32_t amount = (MAX_DIGIT - 2) * (MAX_DIGIT - 2); // There are as many boxes-centers as there are cells
  return amount * MAX_DIGIT; // 9 possible digits for each box-center

  // TODO: 3 (denser boxes)
  // int32_t amount = 4 * 4; //
  // return amount * MAX_DIGIT; // 9 possible digits for each box-center

  // // TODO: 4
  // int32_t amount = 8 * 9; // horizontal dashes
  // // amount+= 8 * 9; // vertical dashes
  // // amount+= 8 * 8; // negative diagonal dashes
  // // amount+= 8 * 8; // positive diagonal dashes
  // return amount * MAX_DIGIT; // positive diagonal dashes

  // // TODO: 5 (extending already-present boxes with dashes)
  // int32_t amount = 2 * 9; // horizontal dashes
  // // amount += 2 * 9; // vertical dashes
  // // amount += 2 * 8; // positve diagonal dashes vertically
  // // amount += 2 * 8; // negative diagonal dashes vertically
  // return amount * MAX_DIGIT;
}

bool KingsMove::getDlxConstraint(Sudo digit, int32_t i, int32_t j, int32_t columnId) const {
  // TODO: 1
  // // columnId encodes the (cell id, possible digit) pair
  // const auto& unpacked = unpackId(columnId, TOTAL_DIGITS, MAX_DIGIT);
  // const int32_t cellId = unpacked.first;
  // const Sudo possibleDigit = static_cast<Sudo>(unpacked.second + 1);

  // for (const int32_t neighboringBoxId : getNeighboringBoxIds(i, j)) {
  //   if (neighboringBoxId == cellId) {
  //     return possibleDigit == digit;
  //   }
  // }
  // return false;

  // TODO: 2 (densest boxes)
  // columnId encodes the (rowId [0-7], columnId [0-7], possible digit) tuple
  const auto& unpacked = unpackId(columnId, MAX_DIGIT - 2, MAX_DIGIT - 2, MAX_DIGIT);

  const int32_t cellId = packId(std::get<0>(unpacked) + 1, std::get<1>(unpacked) + 1, MAX_DIGIT, MAX_DIGIT);

  for (const int32_t neighboringBoxId : getNeighboringBoxIds(i, j)) {
    if (neighboringBoxId == cellId) {
      return static_cast<Sudo>(std::get<2>(unpacked) + 1) == digit;
    }
  }
  return false;

  // TODO: 3 (denser boxes)
  // columnId encodes the (rowId {1,3,5,7}, columnId {1,3,5,7}, possible digit) tuple
  // const auto& unpacked = unpackId(columnId, 4, 4, MAX_DIGIT);

  // const int32_t cellId = packId(std::get<0>(unpacked) * 2 + 1, std::get<1>(unpacked) * 2 + 1, MAX_DIGIT, MAX_DIGIT);
  // for (const int32_t neighboringBoxId : getNeighboringBoxIds(i, j)) {
  //   if (neighboringBoxId == cellId) {
  //     return static_cast<Sudo>(std::get<2>(unpacked) + 1) == digit;
  //   }
  // }
  // return false;

  // TODO: 4
  // columnId encodes the dashId, possible digit pair
  // const auto& [dashId, digitIndex] = unpackId(columnId, 8 * 9, MAX_DIGIT);
  // const Sudo possibleDigit = static_cast<Sudo>(digitIndex + 1);
  // const bool isSame = possibleDigit == digit;

  // if (dashId < 8 * 9) {
  //   const auto& [row, column] = unpackId(dashId, 9, 8);
  //   return isSame && ((i == row && j == column) || (i == row && j == column + 1));
  // }
  // return false;

  // // TODO: 5 (extending already-present boxes with dashes)
  // // columnId encodes the dashId, possible digit pair
  // const auto& [dashId, digitIndex] = unpackId(columnId, 2 * 9, MAX_DIGIT);
  // const Sudo possibleDigit = static_cast<Sudo>(digitIndex + 1);
  // const bool isSame = possibleDigit == digit;

  // if (dashId < 9) { // first column of horizontal lines
  //   const int32_t row = dashId;
  //   constexpr int32_t column = 2;
  //   // const auto& [row, column] = unpackId(dashId, 9, 8);
  //   return isSame && i == row && ((j == column) || (j == column + 1));
  // }
  // if (dashId < 2 * 9) {
  //   const int32_t row = dashId - 9;
  //   constexpr int32_t column = 5;
  //   // const auto& [row, column] = unpackId(dashId, 9, 8);
  //   return isSame && i == row && ((j == column) || (j == column + 1));
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