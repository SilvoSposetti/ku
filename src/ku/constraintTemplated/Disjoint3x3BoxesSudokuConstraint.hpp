#pragma once

#include "Constraint.hpp"
#include "ConstraintMacros.hpp"

GENERATE_CONSTRAINT_TRAITS(Disjoint3x3BoxesSudokuConstraint, 1, 0);

template <PuzzleIntrinsics puzzle>
struct Disjoint3x3BoxesSudokuConstraint : public Constraint<Disjoint3x3BoxesSudokuConstraint<puzzle>, puzzle> {
public:
  constexpr Disjoint3x3BoxesSudokuConstraint()
      : Constraint<Disjoint3x3BoxesSudokuConstraint<puzzle>, puzzle>(
            ConstraintType::DISJOINT_BOXES,
            "Disjoint-Boxes",
            "Cells in the same relative position within all 3x3 boxes contain all the digits exactly once.") {
    CONSTRAINT_CONCEPT_ASSERT(Disjoint3x3BoxesSudokuConstraint, puzzle);
  };

  static constexpr bool supportsPuzzle() {
    return puzzle.isSudoku();
  }

  static constexpr Option<ConstraintTraits<Disjoint3x3BoxesSudokuConstraint<puzzle>>::primarySize>
  primaryOption(uint32_t row, uint32_t column, uint32_t digit) {
    constexpr auto boxSize = 3;
    constexpr auto boxesVerticalCount = static_cast<uint32_t>(puzzle.columns) / boxSize;
    const auto idInBox = (column % boxSize + boxesVerticalCount * (row % boxSize));
    return {static_cast<OptionId>(idInBox * static_cast<uint32_t>(puzzle.digits.size()) + (digit - 1))};
  }

  static constexpr Option<ConstraintTraits<Disjoint3x3BoxesSudokuConstraint<puzzle>>::secondarySize>
  secondaryOption([[maybe_unused]] uint32_t row, [[maybe_unused]] uint32_t column, [[maybe_unused]] uint32_t digit) {
    return {};
  }
};