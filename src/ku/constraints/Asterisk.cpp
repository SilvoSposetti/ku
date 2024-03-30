#include "Asterisk.h"

Asterisk::Asterisk()
    : AbstractConstraint(ConstraintType::ASTERISK,
                         "Asterisk",
                         "The 9 cells forming an asterisk contain all the digits from 1 to 9 exactly once.") {
  cells = {{1, 4}, {2, 2}, {2, 6}, {4, 1}, {4, 4}, {4, 7}, {6, 2}, {6, 6}, {7, 4}};
}

std::string Asterisk::getSvgGroup() const {
  std::string squares;
  const double cellSize = 1.0 / static_cast<double>(Sudo::MAX_DIGIT);

  for (const auto& [i, j] : cells) {
    const double topLeftX = i * cellSize;
    const double topLeftY = j * cellSize;

    squares += SvgUtilities::rect(topLeftX, topLeftY, cellSize, cellSize);
  }
  return SvgUtilities::createGroup(getName(), squares, SvgUtilities::getNoFillStroke(mediumLine));
}

bool Asterisk::satisfy(const std::vector<std::vector<Sudo::Digit>>& board) const {
  for (const auto& d : Sudo::SUDO_DIGITS) {
    int32_t counter = 0;
    for (const auto& [i, j] : cells) {
      if (board[i][j] == d) {
        counter++;
      }
    }
    if (counter != 1) {
      return false;
    }
  }
  return true;
}

int32_t Asterisk::getItemsAmount() const {
  return Sudo::MAX_DIGIT;
}

bool Asterisk::computeConstraint(Sudo::Digit digit, int32_t i, int32_t j, int32_t columnId) const {
  const auto [cellId, digitIndex] = IdPacking::unpackId(columnId, cells.size(), Sudo::MAX_DIGIT);
  const Sudo::Digit possibleDigit = static_cast<Sudo::Digit>(digitIndex + 1);
  const bool isSame = possibleDigit == digit;

  const bool isInCellsSet = cells.find({i, j}) != cells.end();

  return isInCellsSet && isSame;
}
