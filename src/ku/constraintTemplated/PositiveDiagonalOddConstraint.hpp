#pragma once

#include "Constraint.hpp"
#include "ConstraintMacros.hpp"

GENERATE_CONSTRAINT_TRAITS(PositiveDiagonalOddConstraint, 1, 0);

template <PuzzleIntrinsics puzzle>
struct PositiveDiagonalOddConstraint : public Constraint<PositiveDiagonalOddConstraint<puzzle>, puzzle> {
public:
  constexpr PositiveDiagonalOddConstraint()
      : Constraint<PositiveDiagonalOddConstraint<puzzle>, puzzle>(ConstraintType::POSITIVE_DIAGONAL_ODD,
                                                                  "Positive-Diagonal-Odd",
                                                                  "The positive diagonal contains only odd digits.") {
    CONSTRAINT_CONCEPT_ASSERT(PositiveDiagonalOddConstraint, puzzle);
  };

  constexpr static bool supportsPuzzle() {
    return puzzle.isSquare() && puzzle.hasOddDigits();
  }

  constexpr static Option<ConstraintTraits<PositiveDiagonalOddConstraint<puzzle>>::primarySize>
  primaryOption(uint32_t row, uint32_t column, uint32_t digit) {
    if constexpr (puzzle.columns > 0) {
      if (puzzle.isOnPositiveDiagonal(row, column) && Digits::isOdd(digit)) {
        return {static_cast<OptionId>((row))}; // Or column
      }
    }
    return {};
  }

  constexpr static Option<ConstraintTraits<PositiveDiagonalOddConstraint<puzzle>>::secondarySize>
  secondaryOption([[maybe_unused]] uint32_t row, [[maybe_unused]] uint32_t column, [[maybe_unused]] uint32_t digit) {
    return {};
  }
};