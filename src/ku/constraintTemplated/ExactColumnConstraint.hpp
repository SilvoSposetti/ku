#pragma once

#include "Constraint.hpp"
#include "ConstraintMacros.hpp"

GENERATE_CONSTRAINT_TRAITS(ExactColumnConstraint, 1, 0);

template <PuzzleIntrinsics puzzle>
struct ExactColumnConstraint : public Constraint<ExactColumnConstraint<puzzle>, puzzle> {
public:
  constexpr ExactColumnConstraint()
      : Constraint<ExactColumnConstraint<puzzle>, puzzle>(
            ConstraintType::EXACT_COLUMN, "Column", "Columns contain all the digits exactly once.") {
    CONSTRAINT_CONCEPT_ASSERT(ExactColumnConstraint, puzzle);
  };

  constexpr static bool supportsPuzzle() {
    return puzzle.rows == puzzle.digits.size();
  }

  constexpr static Option<ConstraintTraits<ExactColumnConstraint<puzzle>>::primarySize>
  primaryOption([[maybe_unused]] uint32_t row, uint32_t column, uint32_t digit) {
    return {static_cast<OptionId>((column * static_cast<uint32_t>(puzzle.digits.size()) + (digit - 1)))};
  }

  constexpr static Option<ConstraintTraits<ExactColumnConstraint<puzzle>>::secondarySize>
  secondaryOption([[maybe_unused]] uint32_t row, [[maybe_unused]] uint32_t column, [[maybe_unused]] uint32_t digit) {
    return {};
  }
};