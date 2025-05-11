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

  constexpr static Option primaryOption(uint32_t row, uint32_t column, [[maybe_unused]] uint32_t digit) {
    constexpr auto totalCells = static_cast<uint32_t>(puzzle.rows) * static_cast<uint32_t>(puzzle.columns);
    if constexpr (totalCells > 0) {
      return Option{(row * static_cast<uint32_t>(puzzle.columns) + column) % totalCells};
    }
    return {};
  }

  constexpr static Option
  secondaryOption([[maybe_unused]] uint32_t row, [[maybe_unused]] uint32_t column, [[maybe_unused]] uint32_t digit) {
    return {};
  }
};