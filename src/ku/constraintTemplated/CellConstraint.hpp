#pragma once

#include "Constraint.hpp"
#include "ConstraintMacros.hpp"

GENERATE_CONSTRAINT_TRAITS(CellConstraint, 1, 0);

template <PuzzleIntrinsics puzzle>
struct CellConstraint : public Constraint<CellConstraint<puzzle>, puzzle> {
public:
  constexpr CellConstraint()
      : Constraint<CellConstraint<puzzle>, puzzle>(ConstraintType::CELL, "Cell", "Each cell contains a single digit.") {
    CONSTRAINT_CONCEPT_ASSERT(CellConstraint, puzzle);
  };

  constexpr static bool supportsPuzzle() {
    return true;
  }

  constexpr static Option<ConstraintTraits<CellConstraint>::primarySize>
  primaryOption(uint32_t row, uint32_t column, [[maybe_unused]] uint32_t digit) {
    return {static_cast<OptionId>(row * static_cast<uint32_t>(puzzle.columns) + column)};
  }

  constexpr static Option<ConstraintTraits<CellConstraint>::secondarySize>
  secondaryOption([[maybe_unused]] uint32_t row, [[maybe_unused]] uint32_t column, [[maybe_unused]] uint32_t digit) {
    return {};
  }
};