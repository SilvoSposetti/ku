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
    return puzzle.rows == puzzle.digits.size();
  }

  constexpr static OptionsList<puzzle> createPrimaryItems() {
    auto items = OptionsList<puzzle>();
    size_t counter = 0;
    constexpr auto totalCount = puzzle.rows * puzzle.digits.size();
    if constexpr (totalCount > 0) {
      for (const auto& [row, column, digit] : puzzle.allPossibilities) {
        items[counter].push_back(static_cast<int32_t>(row * puzzle.digits.size() + (digit - 1)) % totalCount);
        counter++;
      };
    }
    return items;
  }

  constexpr static OptionsList<puzzle> createSecondaryItems() {
    return OptionsList<puzzle>();
  }
};