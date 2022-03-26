#include "PositiveDiagonal.h"

ConstraintType PositiveDiagonal::getType() const {
  return ConstraintType::POSITIVE_DIAGONAL;
}

std::string PositiveDiagonal::getName() const {
  return "Positive-Diagonal";
}

std::string PositiveDiagonal::getDescription() const {
  return "The positive diagonal contains all the digts from " + std::to_string(MIN_DIGIT) + " to " +
         std::to_string(MAX_DIGIT) + " exactly once.";
}

std::string PositiveDiagonal::getSvgGroup() const {
  const std::string line = SvgUtilities::line(0, 1, 1, 0);
  return SvgUtilities::createGroup(getName(), line, SvgUtilities::getNoFillStroke(thinLine));
}

bool PositiveDiagonal::validatePlacement(Sudo digit,
                                         int32_t rowIndex,
                                         int32_t columnIndex,
                                         const std::vector<std::vector<Sudo>>& board) const {
  // Accept if the placement is not on the main diagonal
  if (!isOnPositiveDiagonal(rowIndex, columnIndex)) {
    return true;
  }
  // Check if the digit exists already in the positive diagonal
  for (const auto& indexPair : getAllPositiveDiagonalIndices()) {
    if (board[indexPair.first][indexPair.second] == digit) {
      return false;
    }
  }
  return true;
}

bool PositiveDiagonal::satisfy(const std::vector<std::vector<Sudo>>& board) const {
  // The board satisfies the constraint if the positive diagonal does not contain duplicate digits
  const std::vector<std::pair<int32_t, int32_t>> indexPairs = getAllPositiveDiagonalIndices();
  for (const auto& digit : SUDO_DIGITS) {
    int32_t counter = 0;
    for (const auto& pair : indexPairs) {
      if (board[pair.first][pair.second] == digit) {
        counter++;
      }
    }
    if (counter != 1) {
      return false;
    }
  }
  return true;
}

int32_t PositiveDiagonal::getDLXConstraintColumnsAmount() const {
  return MAX_DIGIT;
}

bool PositiveDiagonal::getDLXConstraint(Sudo digit, int32_t i, int32_t j, const int32_t columnId) const {

  // Do not consider digits that are not on the positive diagonal
  // if (!) {
  //   return false;
  // }
  // columnId maps to a location on the main diagonal, from top right to bottom left
  return isOnPositiveDiagonal(i, j) && columnId == static_cast<int32_t>(digit) - 1;
}

std::vector<std::pair<int32_t, int32_t>> PositiveDiagonal::getAllPositiveDiagonalIndices() {
  std::vector<std::pair<int32_t, int32_t>> result;
  for (const auto& i : INDICES) {
    result.emplace_back(std::make_pair(i, MAX_INDEX - i));
  }
  return result;
}

bool PositiveDiagonal::isOnPositiveDiagonal(int32_t i, int32_t j) {
  // return i - j == 0; // TODO: this is the NEGATIVE DIAGONAL
  return i + j == MAX_INDEX; // this is the NEGATIVE DIAGONAL
}