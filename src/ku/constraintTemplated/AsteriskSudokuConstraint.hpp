#pragma once

#include "Constraint.hpp"
#include "ConstraintMacros.hpp"

GENERATE_CONSTRAINT_TRAITS(AsteriskSudokuConstraint, 1, 0);

template <PuzzleIntrinsics puzzle>
struct AsteriskSudokuConstraint : public Constraint<AsteriskSudokuConstraint<puzzle>, puzzle> {
public:
  constexpr AsteriskSudokuConstraint()
      : Constraint<AsteriskSudokuConstraint<puzzle>, puzzle>(
            ConstraintType::ASTERISK,
            "Asterisk",
            "The nine cells forming an asterisk contain all the digits exactly once.") {
    CONSTRAINT_CONCEPT_ASSERT(AsteriskSudokuConstraint, puzzle);
  };

  constexpr static bool supportsPuzzle() {
    return puzzle.isSudoku();
  }

  constexpr static Option<ConstraintTraits<AsteriskSudokuConstraint<puzzle>>::primarySize>
  primaryOption(uint32_t row, uint32_t column, uint32_t digit) {
    constexpr std::array<std::pair<Index, Index>, 9> cells = {
        std::make_pair(1, 4), {2, 2}, {2, 6}, {4, 1}, {4, 4}, {4, 7}, {6, 2}, {6, 6}, {7, 4}};
    if (std::ranges::any_of(cells, [&](const auto& pair) { return pair.first == row && pair.second == column; })) {
      return {static_cast<OptionId>(digit - 1)};
    }
    return {};
  }

  constexpr static Option<ConstraintTraits<AsteriskSudokuConstraint<puzzle>>::secondarySize>
  secondaryOption([[maybe_unused]] uint32_t row, [[maybe_unused]] uint32_t column, [[maybe_unused]] uint32_t digit) {
    return {};
  }
};