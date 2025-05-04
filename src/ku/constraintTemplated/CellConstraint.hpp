#pragma once

#include "../constraints/ConstraintType.hpp"
#include "Constraint.hpp"
#include "ConstraintConcept.hpp"

template <PuzzleIntrinsics puzzle>
class CellConstraint : public Constraint<CellConstraint<puzzle>, puzzle> {
public:
  constexpr CellConstraint()
      : Constraint<CellConstraint<puzzle>, puzzle>(ConstraintType::CELL, "Cell", "Each cell contains a single digit.") {
    static_assert(ConstraintConcept<CellConstraint, puzzle>, "CellConstraint does not satisfy ConstraintConcept");
  };

  constexpr static OptionsList<puzzle> createPrimaryItems() {
    auto items = OptionsList<puzzle>();
    size_t counter = 0;
    constexpr auto totalCells = puzzle.rows * puzzle.columns;
    if constexpr (totalCells > 0) {
      for (const auto& [row, column, digit] : puzzle.allPossibilities) {
        items[counter].push_back(static_cast<int32_t>((row * puzzle.columns + column) % totalCells));
        counter++;
      }
    }
    return items;
  }

  constexpr static OptionsList<puzzle> createSecondaryItems() {
    return OptionsList<puzzle>();
  }
};