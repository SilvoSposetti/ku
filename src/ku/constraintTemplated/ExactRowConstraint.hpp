#pragma once

#include "../constraints/ConstraintType.hpp"
#include "Constraint.hpp"
#include "ConstraintConcept.hpp"

template <PuzzleIntrinsics puzzle>
struct ExactRowConstraint : public Constraint<ExactRowConstraint<puzzle>, puzzle> {
public:
  constexpr ExactRowConstraint()
      : Constraint<ExactRowConstraint<puzzle>, puzzle>(
            ConstraintType::EXACT_ROW, "Row", "Rows contain all the digits exactly once.") {
    static_assert(ConstraintConcept<ExactRowConstraint, puzzle>,
                  "ExactRowConstraint does not satisfy ConstraintConcept");
  };

  constexpr static bool supportsPuzzle() {
    return puzzle.columns == puzzle.digits.size();
  }

  constexpr static Option primaryOption(uint32_t row, [[maybe_unused]] uint32_t column, uint32_t digit) {
    return Option{static_cast<OptionId>(row * static_cast<uint32_t>(puzzle.digits.size()) + (digit - 1))};
  }

  constexpr static Option
  secondaryOption([[maybe_unused]] uint32_t row, [[maybe_unused]] uint32_t column, [[maybe_unused]] uint32_t digit) {
    return {};
  }
};