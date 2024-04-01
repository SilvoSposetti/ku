#include "SudokuBox.h"

SudokuBox::SudokuBox()
    : AbstractConstraint(
          ConstraintType::SUDOKU_BOX, "Sudoku-Box", "3x3 boxes contain all the digits from 1 to 9 exactly once.") {}

std::string SudokuBox::getSvgGroup() const {
  constexpr double oneThird = 1.0 / 3.0;
  constexpr double twoThirds = 2.0 / 3.0;

  std::string result;
  result += SvgUtilities::rect(0, 0, oneThird, oneThird);
  result += SvgUtilities::rect(oneThird, 0, oneThird, oneThird);
  result += SvgUtilities::rect(twoThirds, 0, oneThird, oneThird);

  result += SvgUtilities::rect(0, oneThird, oneThird, oneThird);
  result += SvgUtilities::rect(oneThird, oneThird, oneThird, oneThird);
  result += SvgUtilities::rect(twoThirds, oneThird, oneThird, oneThird);

  result += SvgUtilities::rect(0, 0, twoThirds, oneThird);
  result += SvgUtilities::rect(oneThird, twoThirds, oneThird, oneThird);
  result += SvgUtilities::rect(twoThirds, twoThirds, oneThird, oneThird);

  return SvgUtilities::createGroup(getName(), result, SvgUtilities::getNoFillStroke(mediumLine));
}

bool SudokuBox::satisfy(const std::vector<std::vector<Sudo::Digit>>& board) const {
  // The board satisfies the constraint if all boxes do not contain duplicate digits
  std::vector<std::vector<std::pair<int32_t, int32_t>>> boxesIndices = getBoxIndices();
  for (const auto& d : Sudo::SUDO_DIGITS) {
    for (const auto& boxIndexPairs : boxesIndices) {
      int32_t count = 0;
      for (const auto& pair : boxIndexPairs) {
        if (board[pair.first][pair.second] == d) {
          count++;
        }
      }
      if (count != 1) {
        return false;
      }
    }
  }
  return true;
}

int32_t SudokuBox::getBoxId(const int32_t rowIndex, const int32_t columnIndex) {
  // Use integer division to floor results
  return static_cast<int32_t>(columnIndex / 3 + 3 * (rowIndex / 3));
}

std::vector<std::vector<std::pair<int32_t, int32_t>>> SudokuBox::getBoxIndices() {
  std::vector<std::vector<std::pair<int32_t, int32_t>>> allBoxIndexPairs;
  for (int32_t boxIndexRow = 0; boxIndexRow < 3; boxIndexRow++) {
    for (int32_t boxIndexColumn = 0; boxIndexColumn < 3; boxIndexColumn++) {
      std::vector<std::pair<int32_t, int32_t>> singleBoxIndexPairs;
      for (int32_t i = 0; i < 3; i++) {
        for (int32_t j = 0; j < 3; j++) {
          singleBoxIndexPairs.emplace_back(std::make_pair(boxIndexRow * 3 + i, boxIndexColumn * 3 + j));
        }
      }
      allBoxIndexPairs.emplace_back(singleBoxIndexPairs);
    }
  }
  return allBoxIndexPairs;
}

bool SudokuBox::computeConstraint(Sudo::Digit digit, int32_t i, int32_t j, const int32_t itemId) const {
  // columnId encodes the (box id, possible digit) pair
  const std::pair<int32_t, int32_t> unpacked = IdPacking::unpackId(itemId, Sudo::MAX_DIGIT, Sudo::MAX_DIGIT);
  const int32_t boxId = unpacked.first;
  const Sudo::Digit possibleDigit = static_cast<Sudo::Digit>(unpacked.second + 1);

  return boxId == getBoxId(i, j) && possibleDigit == digit;
}

int32_t SudokuBox::getPrimaryItemsAmount() const {
  return 9 * 9; // 9(boxes), 9(possible digits in each box)
}

std::vector<std::vector<int32_t>> SudokuBox::getPrimaryItems() const {
  std::vector<std::vector<int32_t>> primaryItems;

  for (const auto& [i, j, digit] : optionsOrdered()) {
    std::vector<int32_t> items;
    for (int32_t itemId = 0; itemId < getPrimaryItemsAmount(); itemId++) {
      if (computeConstraint(digit, i, j, itemId)) {
        items.emplace_back(itemId);
      }
    }
    primaryItems.push_back(items);
  }
  return primaryItems;
}

int32_t SudokuBox::getSecondaryItemsAmount() const {
  return 0;
}

std::vector<std::vector<int32_t>> SudokuBox::getSecondaryItems() const {
  return{};
}