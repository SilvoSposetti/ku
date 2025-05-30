#pragma once

#include "Constraint.hpp"
#include "ConstraintMacros.hpp"

GENERATE_CONSTRAINT_TRAITS(NegativeDiagonalEvenConstraint, 1, 0);

template <PuzzleIntrinsics puzzle>
struct NegativeDiagonalEvenConstraint : public Constraint<NegativeDiagonalEvenConstraint<puzzle>, puzzle> {
public:
  constexpr NegativeDiagonalEvenConstraint()
      : Constraint<NegativeDiagonalEvenConstraint<puzzle>, puzzle>(ConstraintType::NEGATIVE_DIAGONAL_EVEN,
                                                                   "Negative-Diagonal-Even",
                                                                   "The negative diagonal contains only even digits.") {
    CONSTRAINT_CONCEPT_ASSERT(NegativeDiagonalEvenConstraint, puzzle);
  };

  constexpr static bool supportsPuzzle() {
    return puzzle.isSquare();
  }

  constexpr static Option<ConstraintTraits<NegativeDiagonalEvenConstraint<puzzle>>::primarySize>
  primaryOption(uint32_t row, uint32_t column, uint32_t digit) {
    if constexpr (puzzle.columns > 0) {
      if (puzzle.isOnNegativeDiagonal(row, column) && Digits::isEven(digit)) {
        return {static_cast<OptionId>((row))}; // Or column
      }
    }
    return {};
  }

  constexpr static Option<ConstraintTraits<NegativeDiagonalEvenConstraint<puzzle>>::secondarySize>
  secondaryOption([[maybe_unused]] uint32_t row, [[maybe_unused]] uint32_t column, [[maybe_unused]] uint32_t digit) {
    return {};
  }
};