#pragma once

#include "../constraints/ConstraintType.hpp"
#include "Constraint.hpp"
#include "ConstraintConcept.hpp"

template <PuzzleIntrinsics puzzle>
struct PositiveDiagonalOddConstraint : public Constraint<PositiveDiagonalOddConstraint<puzzle>, puzzle> {
public:
  constexpr PositiveDiagonalOddConstraint()
      : Constraint<PositiveDiagonalOddConstraint<puzzle>, puzzle>(ConstraintType::POSITIVE_DIAGONAL_ODD,
                                                                  "Positive-Diagonal-Odd",
                                                                  "The positive diagonal contains only odd digits.") {
    static_assert(ConstraintConcept<PositiveDiagonalOddConstraint, puzzle>,
                  "PositiveDiagonalOddConstraint does not satisfy ConstraintConcept");
  };

  constexpr static bool supportsPuzzle() {
    return puzzle.isSquare();
  }

  constexpr static Option primaryOption(uint32_t row, uint32_t column, uint32_t digit) {
    if constexpr (puzzle.columns > 0) {
      if (puzzle.isOnPositiveDiagonal(row, column) && Digits::isOdd(digit)) {
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