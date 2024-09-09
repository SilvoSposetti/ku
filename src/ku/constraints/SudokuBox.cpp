#include "SudokuBox.h"

#include "../drawing/SvgRect.h"

SudokuBox::SudokuBox()
    : AbstractConstraint(
          ConstraintType::SUDOKU_BOX, "Sudoku-Box", "3x3 boxes contain all the digits from 1 to 9 exactly once.") {}

std::unique_ptr<SvgGroup> SudokuBox::getSvgGroup(const DrawingOptions& options) const {
  const double oneThird = options.size / 3.0;
  const double twoThirds = oneThird * 2.0;
  auto group = std::make_unique<SvgGroup>(getName(), "transparent", "black", options.mediumLine);
  group->add(std::make_unique<SvgRect>(0, 0, oneThird, oneThird, std::nullopt, std::nullopt, std::nullopt));
  group->add(std::make_unique<SvgRect>(oneThird, 0, oneThird, oneThird, std::nullopt, std::nullopt, std::nullopt));
  group->add(std::make_unique<SvgRect>(twoThirds, 0, oneThird, oneThird, std::nullopt, std::nullopt, std::nullopt));

  group->add(std::make_unique<SvgRect>(0, oneThird, oneThird, oneThird, std::nullopt, std::nullopt, std::nullopt));
  group->add(
      std::make_unique<SvgRect>(oneThird, oneThird, oneThird, oneThird, std::nullopt, std::nullopt, std::nullopt));
  group->add(
      std::make_unique<SvgRect>(twoThirds, oneThird, oneThird, oneThird, std::nullopt, std::nullopt, std::nullopt));

  group->add(std::make_unique<SvgRect>(0, 0, twoThirds, oneThird, std::nullopt, std::nullopt, std::nullopt));
  group->add(
      std::make_unique<SvgRect>(oneThird, twoThirds, oneThird, oneThird, std::nullopt, std::nullopt, std::nullopt));
  group->add(
      std::make_unique<SvgRect>(twoThirds, twoThirds, oneThird, oneThird, std::nullopt, std::nullopt, std::nullopt));
  return group;
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

int32_t SudokuBox::definePrimaryItemsAmount() const {
  return 9 * 9; // 9(boxes), 9(possible digits in each box)
}

std::vector<std::vector<int32_t>> SudokuBox::definePrimaryItems() const {
  std::vector<std::vector<int32_t>> primaryItems(optionsOrdered().size());

  int32_t counter = 0;
  for (const auto& [rowIndex, columnIndex, digit] : optionsOrdered()) {
    primaryItems[counter] = {(getBoxId(rowIndex, columnIndex) * Sudo::MAX_DIGIT + (static_cast<int32_t>(digit) - 1)) %
                             Sudo::TOTAL_DIGITS};
    counter++;
  }
  return primaryItems;
}

int32_t SudokuBox::defineSecondaryItemsAmount() const {
  return 0;
}

std::vector<std::vector<int32_t>> SudokuBox::defineSecondaryItems() const {
  return {};
}