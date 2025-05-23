#pragma once

#include "../constraints/ConstraintType.hpp"
#include "Constraint.hpp"
#include "ConstraintConcept.hpp"

namespace PositiveDiagonalEvenConstraintOptionSizes {
static constexpr std::size_t primary = 1;
static constexpr std::size_t secondary = 0;
} // namespace PositiveDiagonalEvenConstraintOptionSizes

template <PuzzleIntrinsics puzzle>
struct PositiveDiagonalEvenConstraint : public Constraint<PositiveDiagonalEvenConstraint<puzzle>,
                                                          puzzle,
                                                          PositiveDiagonalEvenConstraintOptionSizes::primary,
                                                          PositiveDiagonalEvenConstraintOptionSizes::secondary> {
public:
  constexpr PositiveDiagonalEvenConstraint()
      : Constraint<PositiveDiagonalEvenConstraint<puzzle>,
                   puzzle,
                   PositiveDiagonalEvenConstraintOptionSizes::primary,
                   PositiveDiagonalEvenConstraintOptionSizes::secondary>(
            ConstraintType::POSITIVE_DIAGONAL_EVEN,
            "Positive-Diagonal-Even",
            "The positive diagonal contains only even digits.") {
    static_assert(ConstraintConcept<PositiveDiagonalEvenConstraint,
                                    puzzle,
                                    PositiveDiagonalEvenConstraintOptionSizes::primary,
                                    PositiveDiagonalEvenConstraintOptionSizes::secondary>,
                  "PositiveDiagonalEvenConstraint does not satisfy ConstraintConcept");
  };

  constexpr static bool supportsPuzzle() {
    return puzzle.isSquare();
  }

  constexpr static Option<PositiveDiagonalEvenConstraintOptionSizes::primary>
  primaryOption(uint32_t row, uint32_t column, uint32_t digit) {
    if constexpr (puzzle.columns > 0) {
      if (puzzle.isOnPositiveDiagonal(row, column) && Digits::isEven(digit)) {
        return {static_cast<OptionId>((row))}; // Or column
      }
    }
    return {};
  }

  constexpr static Option<PositiveDiagonalEvenConstraintOptionSizes::secondary>
  secondaryOption([[maybe_unused]] uint32_t row, [[maybe_unused]] uint32_t column, [[maybe_unused]] uint32_t digit) {
    return {};
  }
};