#pragma once

#include "Constraint.hpp"
#include "ConstraintMacros.hpp"

GENERATE_CONSTRAINT_TRAITS(ExactPositiveDiagonalConstraint, 1, 0);

template <PuzzleIntrinsics puzzle>
struct ExactPositiveDiagonalConstraint : public Constraint<ExactPositiveDiagonalConstraint<puzzle>, puzzle> {
public:
  constexpr ExactPositiveDiagonalConstraint()
      : Constraint<ExactPositiveDiagonalConstraint<puzzle>, puzzle>(
            ConstraintType::EXACT_POSITIVE_DIAGONAL,
            "Exact-Positive-Diagonal",
            "The positive diagonal contains all digits exactly once.") {
    CONSTRAINT_CONCEPT_ASSERT(ExactPositiveDiagonalConstraint, puzzle);
  };

  static constexpr bool supportsPuzzle() {
    return puzzle.isSquare() && puzzle.rows == puzzle.digits.size();
  }

  static constexpr Option<ConstraintTraits<ExactPositiveDiagonalConstraint<puzzle>>::primarySize>
  primaryOption(uint32_t row, uint32_t column, uint32_t digit) {
    if constexpr (puzzle.columns > 0) {
      if (puzzle.isOnPositiveDiagonal(row, column)) {
        return {static_cast<OptionId>((digit - 1))};
      }
    }
    return {};
  }

  static constexpr Option<ConstraintTraits<ExactPositiveDiagonalConstraint<puzzle>>::secondarySize>
  secondaryOption([[maybe_unused]] uint32_t row, [[maybe_unused]] uint32_t column, [[maybe_unused]] uint32_t digit) {
    return {};
  }
};