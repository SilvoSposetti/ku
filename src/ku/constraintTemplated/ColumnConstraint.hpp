#pragma once

#include "../constraints/ConstraintType.hpp"
#include "Constraint.hpp"
#include "ConstraintConcept.hpp"

template <PuzzleIntrinsics puzzle>
struct ColumnConstraint : public Constraint<ColumnConstraint<puzzle>, puzzle> {
public:
  constexpr ColumnConstraint()
      : Constraint<ColumnConstraint<puzzle>, puzzle>(
            ConstraintType::EXACT_COLUMN, "Column", "Columns contain all the digits exactly once.") {
    static_assert(ConstraintConcept<ColumnConstraint, puzzle>, "CellConstraint does not satisfy ConstraintConcept");
  };

  constexpr static bool supportsPuzzle() {
    return puzzle.rows == puzzle.digits.size();
  }

  constexpr static std::optional<Option> primaryOption([[maybe_unused]] Index row, Index column, Digit digit) {
    constexpr auto totalCount = static_cast<int32_t>(puzzle.columns) * static_cast<int32_t>(puzzle.digits.size());
    if constexpr (totalCount > 0) {
      return Option{static_cast<int32_t>(static_cast<int32_t>(column) * static_cast<int32_t>(puzzle.digits.size()) +
                                         (static_cast<int32_t>(digit) - 1)) %
                    totalCount};
    }
    return std::nullopt;
  }

  constexpr static std::optional<Option>
  secondaryOption([[maybe_unused]] Index row, [[maybe_unused]] Index column, [[maybe_unused]] Digit digit) {
    return std::nullopt;
  }
};