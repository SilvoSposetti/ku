#pragma once

#include "../constraints/ConstraintType.hpp"
#include "Constraint.hpp"
#include "ConstraintConcept.hpp"

template <PuzzleIntrinsics puzzle>
struct RowConstraint : public Constraint<RowConstraint<puzzle>, puzzle> {
public:
  constexpr RowConstraint()
      : Constraint<RowConstraint<puzzle>, puzzle>(
            ConstraintType::EXACT_ROW, "Row", "Rows contain all the digits exactly once.") {
    static_assert(ConstraintConcept<RowConstraint, puzzle>, "RowConstraint does not satisfy ConstraintConcept");
  };

  constexpr static bool supportsPuzzle() {
    return puzzle.columns == puzzle.digits.size();
  }

  constexpr static std::optional<Option> primaryOption(Index row, [[maybe_unused]] Index column, Digit digit) {
    constexpr auto totalCount = static_cast<int32_t>(puzzle.rows) * static_cast<int32_t>(puzzle.digits.size());
    if constexpr (totalCount > 0) {
      return Option{static_cast<int32_t>(row * puzzle.digits.size() + (digit - 1)) % totalCount};
    }
    return std::nullopt;
  }

  constexpr static std::optional<Option>
  secondaryOption([[maybe_unused]] Index row, [[maybe_unused]] Index column, [[maybe_unused]] Digit digit) {
    return std::nullopt;
  }
};