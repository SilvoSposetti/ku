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

  constexpr static Option primaryOption(uint32_t row, uint32_t column, uint32_t digit) {
    constexpr auto totalCount = static_cast<uint32_t>(puzzle.rows) * static_cast<uint32_t>(puzzle.columns);
    constexpr auto boxesVerticalCount = static_cast<uint32_t>(puzzle.columns) / 3;
    if constexpr (totalCount > 0) {
      const auto boxId = (column / 3 + boxesVerticalCount * (row / 3));
      return Option{
          static_cast<OptionId>((boxId * static_cast<uint32_t>(puzzle.digits.size()) + (digit - 1)) % totalCount)};
    }
    return {};
  }

  constexpr static Option
  secondaryOption([[maybe_unused]] uint32_t row, [[maybe_unused]] uint32_t column, [[maybe_unused]] uint32_t digit) {
    return {};
  }
};