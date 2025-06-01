#pragma once

#include "Constraint.hpp"
#include "ConstraintMacros.hpp"

GENERATE_CONSTRAINT_TRAITS(NegativeDiagonalOddConstraint, 1, 0);

template <PuzzleIntrinsics puzzle>
struct NegativeDiagonalOddConstraint : public Constraint<NegativeDiagonalOddConstraint<puzzle>, puzzle> {
public:
  constexpr NegativeDiagonalOddConstraint()
      : Constraint<NegativeDiagonalOddConstraint<puzzle>, puzzle>(ConstraintType::NEGATIVE_DIAGONAL_ODD,
                                                                  "Negative-Diagonal-Odd",
                                                                  "The negative diagonal contains only odd digits.") {
    CONSTRAINT_CONCEPT_ASSERT(NegativeDiagonalOddConstraint, puzzle);
  };

  constexpr static bool supportsPuzzle() {
    return puzzle.isSquare() && puzzle.hasOddDigits();
  }

  constexpr static Option<ConstraintTraits<NegativeDiagonalOddConstraint<puzzle>>::primarySize>
  primaryOption(uint32_t row, uint32_t column, uint32_t digit) {
    if constexpr (puzzle.columns > 0) {
      if (puzzle.isOnNegativeDiagonal(row, column) && Digits::isOdd(digit)) {
        return {static_cast<OptionId>((row))}; // Or column
      }
    }
    return {};
  }

  constexpr static Option<ConstraintTraits<NegativeDiagonalOddConstraint<puzzle>>::secondarySize>
  secondaryOption([[maybe_unused]] uint32_t row, [[maybe_unused]] uint32_t column, [[maybe_unused]] uint32_t digit) {
    return {};
  }
};