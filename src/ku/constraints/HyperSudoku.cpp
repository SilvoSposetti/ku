#include "HyperSudoku.h"

HyperSudoku::HyperSudoku()
    : AbstractConstraint(ConstraintType::HYPER_SUDOKU,
                         "Hyper-Sudoku",
                         "Four 3x3 boxes contain all the digits from 1 to 9 exactly once.")
    , topLeftCorners({{1, 1}, {5, 1}, {1, 5}, {5, 5}}) {}

std::string HyperSudoku::getSvgGroup() const {
  std::string squares;
  const double cellSize = 1.0 / static_cast<double>(Sudo::MAX_DIGIT);

  for (const auto& [i, j] : topLeftCorners) {
    const double topLeftX = i * cellSize;
    const double topLeftY = j * cellSize;

    squares += SvgUtilities::rect(topLeftX, topLeftY, 3 * cellSize, 3 * cellSize);
  }
  return SvgUtilities::createGroup(getName(), squares, SvgUtilities::getNoFillStroke(mediumLine));
}

bool HyperSudoku::satisfy(const std::vector<std::vector<Sudo::Digit>>& board) const {
  // The board satisfies the constraint if all boxes do not contain duplicate digits
  for (const auto& [topLeftI, topLeftJ] : topLeftCorners) {
    for (const auto digit : Sudo::SUDO_DIGITS) {
      int32_t count = 0;
      for (int32_t i = topLeftI; i < topLeftI + 3; i++) {
        for (int32_t j = topLeftJ; j < topLeftJ + 3; j++) {
          if (board[i][j] == digit) {
            count++;
          }
        }
      }
      if (count != 1) {
        return false;
      }
    }
  }
  return true;
}

int32_t HyperSudoku::getItemsAmount() const {
  return 4 * Sudo::MAX_DIGIT; // 4 boxes, 9 possible digits for each box
}

bool HyperSudoku::isCellInBox(int32_t rowIndex, int32_t columnIndex, int32_t boxId) const {
  // // Use integer division to floor results
  // return static_cast<int32_t>(columnIndex / 3 + 3 * (rowIndex / 3));

  const auto& [i, j] = topLeftCorners[boxId];
  return (i <= rowIndex && rowIndex < (i + 3)) && (j <= columnIndex && columnIndex < (j + 3));
}

bool HyperSudoku::computeConstraint(Sudo::Digit digit, int32_t i, int32_t j, int32_t itemId) const {
  // itemId encodes the (box id, possible digit) pair
  const auto [boxId, possibleDigitEncoded] = IdPacking::unpackId(itemId, 4, Sudo::MAX_DIGIT);
  const Sudo::Digit possibleDigit = static_cast<Sudo::Digit>(possibleDigitEncoded + 1);

  return isCellInBox(i, j, boxId) && possibleDigit == digit;
}
