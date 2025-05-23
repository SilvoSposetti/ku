#pragma once

#include "../constraints/ConstraintType.hpp"
#include "Constraint.hpp"
#include "ConstraintConcept.hpp"

namespace NegativeDiagonalEvenConstraintOptionSizes {
static constexpr std::size_t primary = 1;
static constexpr std::size_t secondary = 0;
} // namespace NegativeDiagonalEvenConstraintOptionSizes

template <PuzzleIntrinsics puzzle>
struct NegativeDiagonalEvenConstraint : public Constraint<NegativeDiagonalEvenConstraint<puzzle>,
                                                          puzzle,
                                                          NegativeDiagonalEvenConstraintOptionSizes::primary,
                                                          NegativeDiagonalEvenConstraintOptionSizes::secondary> {
public:
  constexpr NegativeDiagonalEvenConstraint()
      : Constraint<NegativeDiagonalEvenConstraint<puzzle>,
                   puzzle,
                   NegativeDiagonalEvenConstraintOptionSizes::primary,
                   NegativeDiagonalEvenConstraintOptionSizes::secondary>(
            ConstraintType::NEGATIVE_DIAGONAL_EVEN,
            "Negative-Diagonal-Even",
            "The negative diagonal contains only even digits.") {
    static_assert(ConstraintConcept<NegativeDiagonalEvenConstraint,
                                    puzzle,
                                    NegativeDiagonalEvenConstraintOptionSizes::primary,
                                    NegativeDiagonalEvenConstraintOptionSizes::secondary>,
                  "NegativeDiagonalEvenConstraint does not satisfy ConstraintConcept");
  };

  constexpr static bool supportsPuzzle() {
    return puzzle.isSquare();
  }

  constexpr static Option<NegativeDiagonalEvenConstraintOptionSizes::primary>
  primaryOption(uint32_t row, uint32_t column, uint32_t digit) {
    if constexpr (puzzle.columns > 0) {
      if (puzzle.isOnNegativeDiagonal(row, column) && Digits::isEven(digit)) {
        return {static_cast<OptionId>((row))}; // Or column
      }
    }
    return {};
  }

  constexpr static Option<NegativeDiagonalEvenConstraintOptionSizes::secondary>
  secondaryOption([[maybe_unused]] uint32_t row, [[maybe_unused]] uint32_t column, [[maybe_unused]] uint32_t digit) {
    return {};
  }
};