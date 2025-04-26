#pragma once

#include "../constraints/ConstraintType.hpp"
#include "Constraint.hpp"

template <PuzzleIntrinsics puzzle>
class ColumnConstraint : public Constraint<ColumnConstraint<puzzle>, puzzle> {
public:
  constexpr ColumnConstraint()
      : Constraint<ColumnConstraint<puzzle>, puzzle>(
            ConstraintType::SUDOKU_COLUMN, "Column", "Columns contain all the digits exactly once.") {};

  constexpr static ItemsList<puzzle> createPrimaryItems() {
    auto items = ItemsList<puzzle>();
    size_t counter = 0;
    constexpr auto totalCount = puzzle.columns * puzzle.digits.size();
    for (const auto& [row, column, digit] : puzzle.allPossibilities) {
      items[counter].push_back(static_cast<int32_t>(column * puzzle.digits.size() + (digit - 1)) % totalCount);
      counter++;
    };
    return items;
  }

  constexpr static ItemsList<puzzle> createSecondaryItems() {
    return ItemsList<puzzle>();
  }
};