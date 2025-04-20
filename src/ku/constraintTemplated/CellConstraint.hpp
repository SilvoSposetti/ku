#pragma once

#include "../constraints/ConstraintType.hpp"
#include "Constraint.hpp"

template <PuzzleIntrinsics puzzle>
class CellConstraint : public Constraint<CellConstraint<puzzle>, puzzle> {
public:
  constexpr CellConstraint()
      : Constraint<CellConstraint<puzzle>, puzzle>(ConstraintType::SUDOKU_CELL, "Sudoku-Cell", "Description") {};

  constexpr static ItemsList<puzzle> createPrimaryItems() {
    auto items = ItemsList<puzzle>();
    size_t counter = 0;
    const auto totalDigits = puzzle.rows * puzzle.columns;
    for (const auto& [row, column, digit] : puzzle.possibilities) {
      items[counter] = std::array<int32_t, 1>{static_cast<int32_t>((row * puzzle.columns + column) % totalDigits)};
      counter++;
    }
    return items;
  }

  constexpr static ItemsList<puzzle> createSecondaryItems() {
    const auto items = ItemsList<puzzle>();
    return items;
  }
};