#pragma once

#include "../constraints/ConstraintType.hpp"
#include "Constraint.hpp"
#include "ConstraintConcept.hpp"

namespace AsteriskSudokuConstraintOptionSizes {
static constexpr std::size_t primary = 1;
static constexpr std::size_t secondary = 0;
} // namespace AsteriskSudokuConstraintOptionSizes

template <PuzzleIntrinsics puzzle>
struct AsteriskSudokuConstraint : public Constraint<AsteriskSudokuConstraint<puzzle>,
                                                    puzzle,
                                                    AsteriskSudokuConstraintOptionSizes::primary,
                                                    AsteriskSudokuConstraintOptionSizes::secondary> {
public:
  constexpr AsteriskSudokuConstraint()
      : Constraint<AsteriskSudokuConstraint<puzzle>,
                   puzzle,
                   AsteriskSudokuConstraintOptionSizes::primary,
                   AsteriskSudokuConstraintOptionSizes::secondary>(
            ConstraintType::ASTERISK,
            "Asterisk",
            "The nine cells forming an asterisk contain all the digits exactly once.") {
    static_assert(ConstraintConcept<AsteriskSudokuConstraint,
                                    puzzle,
                                    AsteriskSudokuConstraintOptionSizes::primary,
                                    AsteriskSudokuConstraintOptionSizes::secondary>,
                  "AsteriskSudokuConstraint does not satisfy ConstraintConcepFt");
  };

  constexpr static bool supportsPuzzle() {
    return puzzle.isSudoku();
  }

  constexpr static Option<AsteriskSudokuConstraintOptionSizes::primary>
  primaryOption(uint32_t row, uint32_t column, uint32_t digit) {
    constexpr std::array<std::pair<Index, Index>, 9> cells = {
        std::make_pair(1, 4), {2, 2}, {2, 6}, {4, 1}, {4, 4}, {4, 7}, {6, 2}, {6, 6}, {7, 4}};
    if (std::ranges::contains(cells, std::make_pair(row, column))) {
      return {static_cast<OptionId>(digit - 1)};
    }
    return {};
  }

  constexpr static Option<AsteriskSudokuConstraintOptionSizes::secondary>
  secondaryOption([[maybe_unused]] uint32_t row, [[maybe_unused]] uint32_t column, [[maybe_unused]] uint32_t digit) {
    return {};
  }
};