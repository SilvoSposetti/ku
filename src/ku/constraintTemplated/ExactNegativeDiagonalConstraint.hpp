#pragma once

#include "../constraints/ConstraintType.hpp"
#include "Constraint.hpp"
#include "ConstraintConcept.hpp"

namespace ExactNegativeDiagonalConstraintOptionSizes {
static constexpr std::size_t primary = 1;
static constexpr std::size_t secondary = 0;
} // namespace ExactNegativeDiagonalConstraintOptionSizes

template <PuzzleIntrinsics puzzle>
struct ExactNegativeDiagonalConstraint : public Constraint<ExactNegativeDiagonalConstraint<puzzle>,
                                                           puzzle,
                                                           ExactNegativeDiagonalConstraintOptionSizes::primary,
                                                           ExactNegativeDiagonalConstraintOptionSizes::secondary> {
public:
  constexpr ExactNegativeDiagonalConstraint()
      : Constraint<ExactNegativeDiagonalConstraint<puzzle>,
                   puzzle,
                   ExactNegativeDiagonalConstraintOptionSizes::primary,
                   ExactNegativeDiagonalConstraintOptionSizes::secondary>(
            ConstraintType::EXACT_NEGATIVE_DIAGONAL,
            "Exact-Negative-Diagonal",
            "The negative diagonal contains all digits exactly once.") {
    static_assert(ConstraintConcept<ExactNegativeDiagonalConstraint,
                                    puzzle,
                                    ExactNegativeDiagonalConstraintOptionSizes::primary,
                                    ExactNegativeDiagonalConstraintOptionSizes::secondary>,
                  "ExactNegativeDiagonalConstraint does not satisfy ConstraintConcept");
  };

  constexpr static bool supportsPuzzle() {
    return puzzle.isSquare() && puzzle.rows == puzzle.digits.size();
  }

  constexpr static Option<ExactNegativeDiagonalConstraintOptionSizes::primary>
  primaryOption(uint32_t row, uint32_t column, uint32_t digit) {
    if constexpr (puzzle.columns > 0) {
      if (puzzle.isOnNegativeDiagonal(row, column)) {
        return {static_cast<OptionId>((digit - 1))};
      }
    }
    return {};
  }

  constexpr static Option<ExactNegativeDiagonalConstraintOptionSizes::secondary>
  secondaryOption([[maybe_unused]] uint32_t row, [[maybe_unused]] uint32_t column, [[maybe_unused]] uint32_t digit) {
    return {};
  }
};