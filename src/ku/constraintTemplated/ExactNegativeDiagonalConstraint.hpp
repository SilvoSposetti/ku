#pragma once

#include "../constraints/ConstraintType.hpp"
#include "Constraint.hpp"
#include "ConstraintConcept.hpp"

template <PuzzleIntrinsics puzzle>
struct ExactNegativeDiagonalConstraint : public Constraint<ExactNegativeDiagonalConstraint<puzzle>, puzzle> {
public:
  constexpr ExactNegativeDiagonalConstraint()
      : Constraint<ExactNegativeDiagonalConstraint<puzzle>, puzzle>(
            ConstraintType::EXACT_NEGATIVE_DIAGONAL,
            "Exact-Negative-Diagonal",
            "The negative diagonal contains all digits exactly once.") {
    static_assert(ConstraintConcept<ExactNegativeDiagonalConstraint, puzzle>,
                  "ExactNegativeDiagonalConstraint does not satisfy ConstraintConcept");
  };

  constexpr static bool supportsPuzzle() {
    return puzzle.rows == puzzle.columns && puzzle.rows == puzzle.digits.size();
  }

  constexpr static Option primaryOption(uint32_t row, uint32_t column, [[maybe_unused]] uint32_t digit) {
    if constexpr (puzzle.columns > 0) {
      if (isOnNegativeDiagonal(row, column)) {
        return Option{static_cast<OptionId>((digit - 1))};
      }
    }
    return {};
  }

  constexpr static Option
  secondaryOption([[maybe_unused]] uint32_t row, [[maybe_unused]] uint32_t column, [[maybe_unused]] uint32_t digit) {
    return {};
  }

  constexpr static bool isOnNegativeDiagonal(uint32_t rowIndex, uint32_t columnIndex) {
    return rowIndex == columnIndex;
  }
};