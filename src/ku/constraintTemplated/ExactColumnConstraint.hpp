#pragma once

#include "../constraints/ConstraintType.hpp"
#include "Constraint.hpp"
#include "ConstraintConcept.hpp"

template <PuzzleIntrinsics puzzle>
struct ExactColumnConstraint : public Constraint<ExactColumnConstraint<puzzle>, puzzle> {
public:
  constexpr ExactColumnConstraint()
      : Constraint<ExactColumnConstraint<puzzle>, puzzle>(
            ConstraintType::EXACT_COLUMN, "Column", "Columns contain all the digits exactly once.") {
    static_assert(ConstraintConcept<ExactColumnConstraint, puzzle>, "CellConstraint does not satisfy ConstraintConcept");
  };

  constexpr static bool supportsPuzzle() {
    return puzzle.rows == puzzle.digits.size();
  }

  constexpr static Option primaryOption([[maybe_unused]] uint32_t row, uint32_t column, uint32_t digit) {
    constexpr auto totalCount = static_cast<uint32_t>(puzzle.columns) * static_cast<uint32_t>(puzzle.digits.size());
    if constexpr (totalCount > 0) {
      return Option{
          static_cast<OptionId>((column * static_cast<uint32_t>(puzzle.digits.size()) + (digit - 1)) % totalCount)};
    }
    return {};
  }

  constexpr static Option
  secondaryOption([[maybe_unused]] uint32_t row, [[maybe_unused]] uint32_t column, [[maybe_unused]] uint32_t digit) {
    return {};
  }
};