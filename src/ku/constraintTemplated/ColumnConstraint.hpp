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

  constexpr static OptionsList<puzzle> createPrimaryItems() {
    auto items = OptionsList<puzzle>();
    size_t counter = 0;
    constexpr auto totalCount = puzzle.columns * puzzle.digits.size();
    if constexpr (totalCount > 0) {
      for (const auto& [row, column, digit] : puzzle.allPossibilities) {
        items[counter][0] = static_cast<int32_t>(column * puzzle.digits.size() + (digit - 1)) % totalCount;
        counter++;
      };
    }
    return items;
  }

  constexpr static OptionsList<puzzle> createSecondaryItems() {
    return OptionsList<puzzle>();
  }
};