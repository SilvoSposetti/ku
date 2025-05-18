#pragma once

#include "../constraints/ConstraintType.hpp"
#include "Constraint.hpp"
#include "ConstraintConcept.hpp"

template <PuzzleIntrinsics puzzle>
struct ExactPositiveDiagonalConstraint : public Constraint<ExactPositiveDiagonalConstraint<puzzle>, puzzle> {
public:
  constexpr ExactPositiveDiagonalConstraint()
      : Constraint<ExactPositiveDiagonalConstraint<puzzle>, puzzle>(
            ConstraintType::EXACT_POSITIVE_DIAGONAL,
            "Exact-Positive-Diagonal",
            "The positive diagonal contains all digits exactly once.") {
    static_assert(ConstraintConcept<ExactPositiveDiagonalConstraint, puzzle>,
                  "ExactPositiveDiagonalConstraint does not satisfy ConstraintConcept");
  };

  constexpr static bool supportsPuzzle() {
    return puzzle.rows == puzzle.columns && puzzle.rows == puzzle.digits.size();
  }

  constexpr static Option primaryOption(uint32_t row, uint32_t column, [[maybe_unused]] uint32_t digit) {
    if constexpr (puzzle.columns > 0) {
      if (puzzle.isOnPositiveDiagonal(row, column)) {
        return Option{static_cast<OptionId>((digit - 1))};
      }
    }
    return {};
  }

  constexpr static Option
  secondaryOption([[maybe_unused]] uint32_t row, [[maybe_unused]] uint32_t column, [[maybe_unused]] uint32_t digit) {
    return {};
  }
};