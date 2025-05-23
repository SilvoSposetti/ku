#pragma once

#include "../constraints/ConstraintType.hpp"
#include "Constraint.hpp"
#include "ConstraintConcept.hpp"

namespace ExactPositiveDiagonalConstraintOptionSizes {
static constexpr std::size_t primary = 1;
static constexpr std::size_t secondary = 0;
} // namespace ExactPositiveDiagonalConstraintOptionSizes

template <PuzzleIntrinsics puzzle>
struct ExactPositiveDiagonalConstraint : public Constraint<ExactPositiveDiagonalConstraint<puzzle>,
                                                           puzzle,
                                                           ExactPositiveDiagonalConstraintOptionSizes::primary,
                                                           ExactPositiveDiagonalConstraintOptionSizes::secondary> {
public:
  constexpr ExactPositiveDiagonalConstraint()
      : Constraint<ExactPositiveDiagonalConstraint<puzzle>,
                   puzzle,
                   ExactPositiveDiagonalConstraintOptionSizes::primary,
                   ExactPositiveDiagonalConstraintOptionSizes::secondary>(
            ConstraintType::EXACT_POSITIVE_DIAGONAL,
            "Exact-Positive-Diagonal",
            "The positive diagonal contains all digits exactly once.") {
    static_assert(ConstraintConcept<ExactPositiveDiagonalConstraint,
                                    puzzle,
                                    ExactPositiveDiagonalConstraintOptionSizes::primary,
                                    ExactPositiveDiagonalConstraintOptionSizes::secondary>,
                  "ExactPositiveDiagonalConstraint does not satisfy ConstraintConcept");
  };

  constexpr static bool supportsPuzzle() {
    return puzzle.isSquare() && puzzle.rows == puzzle.digits.size();
  }

  constexpr static Option<ExactPositiveDiagonalConstraintOptionSizes::primary>
  primaryOption(uint32_t row, uint32_t column, uint32_t digit) {
    if constexpr (puzzle.columns > 0) {
      if (puzzle.isOnPositiveDiagonal(row, column)) {
        return {static_cast<OptionId>((digit - 1))};
      }
    }
    return {};
  }

  constexpr static Option<ExactPositiveDiagonalConstraintOptionSizes::secondary>
  secondaryOption([[maybe_unused]] uint32_t row, [[maybe_unused]] uint32_t column, [[maybe_unused]] uint32_t digit) {
    return {};
  }
};