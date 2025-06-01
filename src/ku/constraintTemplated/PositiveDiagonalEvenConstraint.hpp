#pragma once

#include "Constraint.hpp"
#include "ConstraintMacros.hpp"

GENERATE_CONSTRAINT_TRAITS(PositiveDiagonalEvenConstraint, 1, 0);

template <PuzzleIntrinsics puzzle>
struct PositiveDiagonalEvenConstraint : public Constraint<PositiveDiagonalEvenConstraint<puzzle>, puzzle> {
public:
  constexpr PositiveDiagonalEvenConstraint()
      : Constraint<PositiveDiagonalEvenConstraint<puzzle>, puzzle>(ConstraintType::POSITIVE_DIAGONAL_EVEN,
                                                                   "Positive-Diagonal-Even",
                                                                   "The positive diagonal contains only even digits.") {
    CONSTRAINT_CONCEPT_ASSERT(PositiveDiagonalEvenConstraint, puzzle);
  };

  constexpr static bool supportsPuzzle() {
    return puzzle.isSquare() && puzzle.hasEvenDigits();
  }

  constexpr static Option<ConstraintTraits<PositiveDiagonalEvenConstraint<puzzle>>::primarySize>
  primaryOption(uint32_t row, uint32_t column, uint32_t digit) {
    if constexpr (puzzle.columns > 0) {
      if (puzzle.isOnPositiveDiagonal(row, column) && Digits::isEven(digit)) {
        return {static_cast<OptionId>((row))}; // Or column
      }
    }
    return {};
  }

  constexpr static Option<ConstraintTraits<PositiveDiagonalEvenConstraint<puzzle>>::secondarySize>
  secondaryOption([[maybe_unused]] uint32_t row, [[maybe_unused]] uint32_t column, [[maybe_unused]] uint32_t digit) {
    return {};
  }
};