#pragma once

#include "../constraints/ConstraintType.hpp"
#include "Constraint.hpp"
#include "ConstraintConcept.hpp"

namespace ExactRowConstraintOptionSizes {
static constexpr std::size_t primary = 1;
static constexpr std::size_t secondary = 0;
} // namespace ExactRowConstraintOptionSizes

template <PuzzleIntrinsics puzzle>
struct ExactRowConstraint : public Constraint<ExactRowConstraint<puzzle>,
                                              puzzle,
                                              ExactRowConstraintOptionSizes::primary,
                                              ExactRowConstraintOptionSizes::secondary> {
public:
  constexpr ExactRowConstraint()
      : Constraint<ExactRowConstraint<puzzle>,
                   puzzle,
                   ExactRowConstraintOptionSizes::primary,
                   ExactRowConstraintOptionSizes::secondary>(
            ConstraintType::EXACT_ROW, "Row", "Rows contain all the digits exactly once.") {
    static_assert(ConstraintConcept<ExactRowConstraint,
                                    puzzle,
                                    ExactRowConstraintOptionSizes::primary,
                                    ExactRowConstraintOptionSizes::secondary>,
                  "ExactRowConstraint does not satisfy ConstraintConcept");
  };

  constexpr static bool supportsPuzzle() {
    return puzzle.columns == puzzle.digits.size();
  }

  constexpr static Option<ExactRowConstraintOptionSizes::primary>
  primaryOption(uint32_t row, [[maybe_unused]] uint32_t column, uint32_t digit) {
    return {static_cast<OptionId>(row * static_cast<uint32_t>(puzzle.digits.size()) + (digit - 1))};
  }

  constexpr static Option<ExactRowConstraintOptionSizes::secondary>
  secondaryOption([[maybe_unused]] uint32_t row, [[maybe_unused]] uint32_t column, [[maybe_unused]] uint32_t digit) {
    return {};
  }
};