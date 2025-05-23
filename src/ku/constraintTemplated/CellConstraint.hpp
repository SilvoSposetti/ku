#pragma once

#include "../constraints/ConstraintType.hpp"
#include "Constraint.hpp"
#include "ConstraintConcept.hpp"

namespace CellConstraintOptionSizes {
static constexpr std::size_t primary = 1;
static constexpr std::size_t secondary = 0;
} // namespace CellConstraintOptionSizes

template <PuzzleIntrinsics puzzle>
struct CellConstraint : public Constraint<CellConstraint<puzzle>,
                                          puzzle,
                                          CellConstraintOptionSizes::primary,
                                          CellConstraintOptionSizes::secondary> {
public:
  constexpr CellConstraint()
      : Constraint<CellConstraint<puzzle>,
                   puzzle,
                   CellConstraintOptionSizes::primary,
                   CellConstraintOptionSizes::secondary>(
            ConstraintType::CELL, "Cell", "Each cell contains a single digit.") {
    static_assert(ConstraintConcept<CellConstraint,
                                    puzzle,
                                    CellConstraintOptionSizes::primary,
                                    CellConstraintOptionSizes::secondary>,
                  "CellConstraint does not satisfy ConstraintConcept");
  };

  constexpr static bool supportsPuzzle() {
    return true;
  }

  constexpr static Option<CellConstraintOptionSizes::primary>
  primaryOption(uint32_t row, uint32_t column, [[maybe_unused]] uint32_t digit) {
    return {static_cast<OptionId>(row * static_cast<uint32_t>(puzzle.columns) + column)};
  }

  constexpr static Option<CellConstraintOptionSizes::secondary>
  secondaryOption([[maybe_unused]] uint32_t row, [[maybe_unused]] uint32_t column, [[maybe_unused]] uint32_t digit) {
    return {};
  }
};