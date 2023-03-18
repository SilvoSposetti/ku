#include "DisjointBoxes.h"

ConstraintType DisjointBoxes::getType() const {
  return ConstraintType::DISJOINT_BOXES;
}

std::string DisjointBoxes::getDescription() const {

  return "Cells in the same relative position within all 3x3 boxes contain all the digits from 1 to 9 exactly once.";
}

std::string DisjointBoxes::getSvgGroup() const {
  constexpr double squareSize = 1.0 / 168.0;
  constexpr double cellSize = 1.0 / static_cast<double>(MAX_DIGIT);
  constexpr double squareOffset = cellSize * 0.5 * .8;

  std::string result;
  for (int i = 0; i <= MAX_INDEX; i++) {
    for (int j = 0; j <= MAX_INDEX; j++) {
      const double positionX = cellSize * (i + .5) + squareOffset * ((i % 3) - 1);
      const double positionY = cellSize * (j + .5) + squareOffset * ((j % 3) - 1);

      result += SvgUtilities::rotatedRect(positionX, positionY, squareSize, squareSize, M_PI / 4);
    }
  }

  return SvgUtilities::createGroup(getName(), result, SvgUtilities::getNoFillStroke(thinLine));
}

bool DisjointBoxes::satisfy(const std::vector<std::vector<Sudo>>& board) const {
  for (const auto& digit : SUDO_DIGITS) {
    for (int32_t i = 0; i < 3; i++) {
      for (int32_t j = 0; j < 3; j++) {
        int32_t counter = 0;
        for (int addI = 0; addI < 9; addI += 3) {
          for (int addJ = 0; addJ < 9; addJ += 3) {
            if (board[i + addI][j + addJ] == digit) {
              counter++;
            }
          }
        }
        if (counter != 1) {
          std::cout << static_cast<int>(digit) << std::endl;
          return false;
        }
      }
    }
  }
  return true;
}

int32_t DisjointBoxes::getDlxConstraintColumnsAmount() const {
  return 9 * MAX_DIGIT; // 9(places within each box), 9(possible digits in each box)
}

bool DisjointBoxes::getDlxConstraint(Sudo digit, int32_t i, int32_t j, const int32_t columnId) const {
  // columnId encodes the (cell ID within a box, possible digit) pair
  const std::pair<int32_t, int32_t> unpacked = IdPacking::unpackId(columnId, MAX_DIGIT, MAX_DIGIT);
  const int32_t cellIdWithinBox = unpacked.first;
  const Sudo possibleDigit = static_cast<Sudo>(unpacked.second + 1);

  const bool isOnSameBoxRow = i % 3 == cellIdWithinBox / 3;
  const bool isOnSameBoxColumn = j % 3 == cellIdWithinBox % 3;

  return isOnSameBoxRow && isOnSameBoxColumn && possibleDigit == digit;
}
