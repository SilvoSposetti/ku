#pragma once

#include "../constraints/ConstraintType.hpp"
#include "Constraint.hpp"
#include "ConstraintConcept.hpp"

#include <ranges>

template <PuzzleIntrinsics puzzle>
struct HyperSudokuConstraint : public Constraint<HyperSudokuConstraint<puzzle>, puzzle> {
public:
  constexpr HyperSudokuConstraint()
      : Constraint<HyperSudokuConstraint<puzzle>, puzzle>(
            ConstraintType::HYPER_SUDOKU, "Hyper-Sudoku", "Four 3x3 boxes contain all the digits exactly once.") {
    static_assert(ConstraintConcept<HyperSudokuConstraint, puzzle>,
                  "HyperSudokuConstraint does not satisfy ConstraintConcept");
  };

  constexpr static bool supportsPuzzle() {
    return puzzle.isSudoku();
  }

  constexpr static Option primaryOption(uint32_t row, uint32_t column, uint32_t digit) {
    constexpr std::array<std::pair<Index, Index>, 4> topLeftCorners = {
        std::make_pair(1, 1), std::make_pair(1, 5), std::make_pair(5, 1), std::make_pair(5, 5)};
    constexpr Index boxSize = 3;
    for (const auto& [boxId, topLeftCorner] : std::views::enumerate(topLeftCorners)) {
      if ((row - topLeftCorner.first) < boxSize && (column - topLeftCorner.second) < boxSize) {
        return Option{static_cast<OptionId>(boxId * puzzle.digits.size() + (digit - 1))};
      }
    }
    return {};
  }

  constexpr static Option
  secondaryOption([[maybe_unused]] uint32_t row, [[maybe_unused]] uint32_t column, [[maybe_unused]] uint32_t digit) {
    return {};
  }
};