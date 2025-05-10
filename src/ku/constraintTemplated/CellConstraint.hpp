#pragma once

#include "../constraints/ConstraintType.hpp"
#include "Constraint.hpp"
#include "ConstraintConcept.hpp"

template <PuzzleIntrinsics puzzle>
struct CellConstraint : public Constraint<CellConstraint<puzzle>, puzzle> {
public:
  constexpr CellConstraint()
      : Constraint<CellConstraint<puzzle>, puzzle>(ConstraintType::CELL, "Cell", "Each cell contains a single digit.") {
    static_assert(ConstraintConcept<CellConstraint, puzzle>, "CellConstraint does not satisfy ConstraintConcept");
  };

  constexpr static bool supportsPuzzle() {
    return true;
  }

  constexpr static std::optional<Option> primaryOption(Index row, Index column, [[maybe_unused]] Digit digit) {
    constexpr auto totalCells = static_cast<int32_t>(puzzle.rows) * static_cast<int32_t>(puzzle.columns);
    if constexpr (totalCells > 0) {
      return Option{(static_cast<int32_t>(row) * puzzle.columns + column) % totalCells};
    }
    return std::nullopt;
  }

  constexpr static std::optional<Option>
  secondaryOption([[maybe_unused]] Index row, [[maybe_unused]] Index column, [[maybe_unused]] Digit digit) {
    return std::nullopt;
  }
};