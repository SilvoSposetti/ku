#pragma once

#include "../constraints/ConstraintType.hpp"
#include "Constraint.hpp"
#include "ConstraintConcept.hpp"

template <PuzzleIntrinsics puzzle>
struct Disjoint3x3BoxesSudokuConstraint : public Constraint<Disjoint3x3BoxesSudokuConstraint<puzzle>, puzzle> {
public:
  constexpr Disjoint3x3BoxesSudokuConstraint()
      : Constraint<Disjoint3x3BoxesSudokuConstraint<puzzle>, puzzle>(
            ConstraintType::DISJOINT_BOXES,
            "Disjoint-Boxes",
            "Cells in the same relative position within all 3x3 boxes contain all the digits exactly once.") {
    static_assert(ConstraintConcept<Disjoint3x3BoxesSudokuConstraint, puzzle>,
                  "Disjoint3x3BoxesSudokuConstraint does not satisfy ConstraintConcept");
  };

  constexpr static bool supportsPuzzle() {
    return puzzle.isSudoku();
  }

  constexpr static Option primaryOption(uint32_t row, uint32_t column, uint32_t digit) {
    constexpr auto boxSize = 3;
    constexpr auto boxesVerticalCount = static_cast<uint32_t>(puzzle.columns) / boxSize;
    const auto idInBox = (column % boxSize + boxesVerticalCount * (row % boxSize));
    return Option{static_cast<OptionId>(idInBox * static_cast<uint32_t>(puzzle.digits.size()) + (digit - 1))};
  }

  constexpr static Option
  secondaryOption([[maybe_unused]] uint32_t row, [[maybe_unused]] uint32_t column, [[maybe_unused]] uint32_t digit) {
    return {};
  }
};