#pragma once

#include "../constraints/ConstraintType.hpp"
#include "Constraint.hpp"
#include "ConstraintConcept.hpp"

template <PuzzleIntrinsics puzzle>
struct NegativeDiagonalEvenConstraint : public Constraint<NegativeDiagonalEvenConstraint<puzzle>, puzzle> {
public:
  constexpr NegativeDiagonalEvenConstraint()
      : Constraint<NegativeDiagonalEvenConstraint<puzzle>, puzzle>(ConstraintType::NEGATIVE_DIAGONAL_EVEN,
                                                                   "Negative-Diagonal-Even",
                                                                   "The negative diagonal contains only even digits.") {
    static_assert(ConstraintConcept<NegativeDiagonalEvenConstraint, puzzle>,
                  "NegativeDiagonalEvenConstraint does not satisfy ConstraintConcept");
  };

  constexpr static bool supportsPuzzle() {
    return puzzle.isSquare();
  }

  constexpr static Option primaryOption(uint32_t row, uint32_t column, uint32_t digit) {
    if constexpr (puzzle.columns > 0) {
      if (puzzle.isOnNegativeDiagonal(row, column) && Digits::isEven(digit)) {
        return Option{static_cast<OptionId>((row))}; // Or column
      }
    }
    return {};
  }

  constexpr static Option
  secondaryOption([[maybe_unused]] uint32_t row, [[maybe_unused]] uint32_t column, [[maybe_unused]] uint32_t digit) {
    return {};
  }
};