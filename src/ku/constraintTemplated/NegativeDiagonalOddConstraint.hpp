#pragma once

#include "../constraints/ConstraintType.hpp"
#include "Constraint.hpp"
#include "ConstraintConcept.hpp"

namespace NegativeDiagonalOddConstraintOptionSizes {
static constexpr std::size_t primary = 1;
static constexpr std::size_t secondary = 0;
} // namespace NegativeDiagonalOddConstraintOptionSizes

template <PuzzleIntrinsics puzzle>
struct NegativeDiagonalOddConstraint : public Constraint<NegativeDiagonalOddConstraint<puzzle>,
                                                         puzzle,
                                                         NegativeDiagonalOddConstraintOptionSizes::primary,
                                                         NegativeDiagonalOddConstraintOptionSizes::secondary> {
public:
  constexpr NegativeDiagonalOddConstraint()
      : Constraint<NegativeDiagonalOddConstraint<puzzle>,
                   puzzle,
                   NegativeDiagonalOddConstraintOptionSizes::primary,
                   NegativeDiagonalOddConstraintOptionSizes::secondary>(
            ConstraintType::NEGATIVE_DIAGONAL_ODD,
            "Negative-Diagonal-Odd",
            "The negative diagonal contains only odd digits.") {
    static_assert(ConstraintConcept<NegativeDiagonalOddConstraint,
                                    puzzle,
                                    NegativeDiagonalOddConstraintOptionSizes::primary,
                                    NegativeDiagonalOddConstraintOptionSizes::secondary>,
                  "NegativeDiagonalOddConstraint does not satisfy ConstraintConcept");
  };

  constexpr static bool supportsPuzzle() {
    return puzzle.isSquare();
  }

  constexpr static Option<NegativeDiagonalOddConstraintOptionSizes::primary>
  primaryOption(uint32_t row, uint32_t column, uint32_t digit) {
    if constexpr (puzzle.columns > 0) {
      if (puzzle.isOnNegativeDiagonal(row, column) && Digits::isOdd(digit)) {
        return {static_cast<OptionId>((row))}; // Or column
      }
    }
    return {};
  }

  constexpr static Option<NegativeDiagonalOddConstraintOptionSizes::secondary>
  secondaryOption([[maybe_unused]] uint32_t row, [[maybe_unused]] uint32_t column, [[maybe_unused]] uint32_t digit) {
    return {};
  }
};