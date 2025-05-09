#pragma once

#include "../constraints/ConstraintType.hpp"
#include "Constraint.hpp"
#include "ConstraintConcept.hpp"

template <PuzzleIntrinsics puzzle>
struct Exact3x3BoxesConstraint : public Constraint<Exact3x3BoxesConstraint<puzzle>, puzzle> {
public:
  constexpr Exact3x3BoxesConstraint()
      : Constraint<Exact3x3BoxesConstraint<puzzle>, puzzle>(
            ConstraintType::EXACT_3x3_BOXES, "Exact 3x3 Box", "3x3 boxes contain all the digits exactly once.") {
    static_assert(ConstraintConcept<Exact3x3BoxesConstraint, puzzle>,
                  "Box3x3Constraint does not satisfy ConstraintConcept");
  };

  constexpr static bool supportsPuzzle() {
    return puzzle.rows % 3 == 0 && puzzle.columns % 3 == 0 && puzzle.digits.size() == 9;
  }

  constexpr static OptionsList<puzzle> createPrimaryItems() {
    auto items = OptionsList<puzzle>();
    size_t counter = 0;
    constexpr auto totalCount = static_cast<int32_t>(puzzle.rows) * static_cast<int32_t>(puzzle.columns);
    constexpr auto boxesVerticalCount = puzzle.columns / 3;
    if constexpr (totalCount > 0) {
      for (const auto& [row, column, digit] : puzzle.allPossibilities) {
        // Next line explicitly uses integer division to floor results
        const auto boxId = static_cast<int32_t>(column / 3 + boxesVerticalCount * (row / 3));
        items[counter].push_back((boxId * puzzle.digits.size() + (static_cast<int32_t>(digit) - 1)) % totalCount);
        counter++;
      };
    }
    return items;
  }

  constexpr static OptionsList<puzzle> createSecondaryItems() {
    return OptionsList<puzzle>();
  }
};