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

  constexpr static std::optional<Option> primaryOption([[maybe_unused]] Index row, Index column, Digit digit) {
    constexpr auto totalCount = static_cast<int32_t>(puzzle.rows) * static_cast<int32_t>(puzzle.columns);
    constexpr auto boxesVerticalCount = puzzle.columns / 3;
    if constexpr (totalCount > 0) {
      const auto boxId = static_cast<int32_t>(column / 3 + boxesVerticalCount * (row / 3));
      return Option{(boxId * static_cast<int32_t>(puzzle.digits.size()) + (static_cast<int32_t>(digit) - 1)) %
                    totalCount};
    }
    return std::nullopt;
  }

  constexpr static std::optional<Option>
  secondaryOption([[maybe_unused]] Index row, [[maybe_unused]] Index column, [[maybe_unused]] Digit digit) {
    return std::nullopt;
  }
};