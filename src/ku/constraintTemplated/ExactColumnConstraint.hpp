#pragma once

#include "../constraints/ConstraintType.hpp"
#include "Constraint.hpp"
#include "ConstraintConcept.hpp"

namespace ExactColumnConstraintOptionSizes {
static constexpr std::size_t primary = 1;
static constexpr std::size_t secondary = 0;
} // namespace ExactColumnConstraintOptionSizes

template <PuzzleIntrinsics puzzle>
struct ExactColumnConstraint : public Constraint<ExactColumnConstraint<puzzle>,
                                                 puzzle,
                                                 ExactColumnConstraintOptionSizes::primary,
                                                 ExactColumnConstraintOptionSizes::secondary> {
public:
  constexpr ExactColumnConstraint()
      : Constraint<ExactColumnConstraint<puzzle>,
                   puzzle,
                   ExactColumnConstraintOptionSizes::primary,
                   ExactColumnConstraintOptionSizes::secondary>(
            ConstraintType::EXACT_COLUMN, "Column", "Columns contain all the digits exactly once.") {
    static_assert(ConstraintConcept<ExactColumnConstraint,
                                    puzzle,
                                    ExactColumnConstraintOptionSizes::primary,
                                    ExactColumnConstraintOptionSizes::secondary>,
                  "CellConstraint does not satisfy ConstraintConcept");
  };

  constexpr static bool supportsPuzzle() {
    return puzzle.rows == puzzle.digits.size();
  }

  constexpr static Option<ExactColumnConstraintOptionSizes::primary>
  primaryOption([[maybe_unused]] uint32_t row, uint32_t column, uint32_t digit) {
    return {static_cast<OptionId>((column * static_cast<uint32_t>(puzzle.digits.size()) + (digit - 1)))};
  }

  constexpr static Option<ExactColumnConstraintOptionSizes::secondary>
  secondaryOption([[maybe_unused]] uint32_t row, [[maybe_unused]] uint32_t column, [[maybe_unused]] uint32_t digit) {
    return {};
  }
};