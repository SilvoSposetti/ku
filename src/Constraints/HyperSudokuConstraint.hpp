#pragma once

#include "Constraint.hpp"
#include "ConstraintMacros.hpp"
#include "SvgRect.hpp"

#include <ranges>

GENERATE_CONSTRAINT_TRAITS(HyperSudokuConstraint, 1, 0);

template <PuzzleIntrinsics puzzle>
struct HyperSudokuConstraint : public Constraint<HyperSudokuConstraint<puzzle>, puzzle> {
public:
  constexpr HyperSudokuConstraint()
      : Constraint<HyperSudokuConstraint<puzzle>, puzzle>(
            ConstraintType::HYPER_SUDOKU, "Hyper-Sudoku", "Four 3x3 boxes contain all the digits exactly once.") {
    CONSTRAINT_CONCEPT_ASSERT(HyperSudokuConstraint, puzzle);
  };

  static constexpr bool supportsPuzzle() {
    return puzzle.isSudoku();
  }

  static constexpr Option<ConstraintTraits<HyperSudokuConstraint<puzzle>>::primarySize>
  primaryOption(uint32_t row, uint32_t column, uint32_t digit) {

    for (const auto& [boxId, topLeftCorner] : std::views::enumerate(topLeftCorners)) {
      if ((row - topLeftCorner.first) < boxSize && (column - topLeftCorner.second) < boxSize) {
        return {static_cast<OptionId>(boxId * puzzle.digits.size() + (digit - 1))};
      }
    }
    return {};
  }

  static constexpr Option<ConstraintTraits<HyperSudokuConstraint<puzzle>>::secondarySize>
  secondaryOption([[maybe_unused]] uint32_t row, [[maybe_unused]] uint32_t column, [[maybe_unused]] uint32_t digit) {
    return {};
  }

  virtual std::unique_ptr<SvgGroup>
  getSvgGroup(const DrawingOptionsTemplated<puzzle.getPuzzleSpace()>& options) const override {
    auto group = std::make_unique<SvgGroup>(this->getName(), "transparent", "black", options.mediumLine);
    for (const auto& [i, j] : topLeftCorners) {
      const double topLeftX = i * options.cellSize;
      const double topLeftY = j * options.cellSize;
      group->add(std::make_unique<SvgRect>(topLeftX, topLeftY, 3 * options.cellSize, 3 * options.cellSize));
    }
    return group;
  }

private:
  /// The box size
  static constexpr std::size_t boxSize = 3;

  /// The grid-index locations of the top left corners for each box
  static constexpr std::array<std::pair<Index, Index>, 4> topLeftCorners = {
      std::make_pair(1, 1), std::make_pair(1, 5), std::make_pair(5, 1), std::make_pair(5, 5)};
};
