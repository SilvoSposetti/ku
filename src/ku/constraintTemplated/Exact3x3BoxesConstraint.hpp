#pragma once

#include "../constraints/ConstraintType.hpp"
#include "Constraint.hpp"
#include "ConstraintConcept.hpp"

namespace Exact3x3BoxesConstraintOptionSizes {
static constexpr std::size_t primary = 1;
static constexpr std::size_t secondary = 0;
} // namespace Exact3x3BoxesConstraintOptionSizes

template <PuzzleIntrinsics puzzle>
struct Exact3x3BoxesConstraint : public Constraint<Exact3x3BoxesConstraint<puzzle>,
                                                   puzzle,
                                                   Exact3x3BoxesConstraintOptionSizes::primary,
                                                   Exact3x3BoxesConstraintOptionSizes::secondary> {
public:
  constexpr Exact3x3BoxesConstraint()
      : Constraint<Exact3x3BoxesConstraint<puzzle>,
                   puzzle,
                   Exact3x3BoxesConstraintOptionSizes::primary,
                   Exact3x3BoxesConstraintOptionSizes::secondary>(
            ConstraintType::EXACT_3x3_BOXES, "Exact 3x3 Box", "3x3 boxes contain all the digits exactly once.") {
    static_assert(ConstraintConcept<Exact3x3BoxesConstraint,
                                    puzzle,
                                    Exact3x3BoxesConstraintOptionSizes::primary,
                                    Exact3x3BoxesConstraintOptionSizes::secondary>,
                  "Box3x3Constraint does not satisfy ConstraintConcept");
  };

  constexpr static bool supportsPuzzle() {
    constexpr auto boxSize = 3;
    return puzzle.rows % boxSize == 0 && puzzle.columns % boxSize == 0 && puzzle.digits.size() == 9;
  }

  constexpr static Option<Exact3x3BoxesConstraintOptionSizes::primary>
  primaryOption(uint32_t row, uint32_t column, uint32_t digit) {
    constexpr auto boxSize = 3;
    constexpr auto boxesVerticalCount = static_cast<uint32_t>(puzzle.columns) / boxSize;
    const auto boxId = (column / boxSize + boxesVerticalCount * (row / boxSize));
    return {static_cast<OptionId>(boxId * static_cast<uint32_t>(puzzle.digits.size()) + (digit - 1))};
  }

  constexpr static Option<Exact3x3BoxesConstraintOptionSizes::secondary>
  secondaryOption([[maybe_unused]] uint32_t row, [[maybe_unused]] uint32_t column, [[maybe_unused]] uint32_t digit) {
    return {};
  }
};