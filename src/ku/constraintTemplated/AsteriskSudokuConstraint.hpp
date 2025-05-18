#pragma once

#include "../constraints/ConstraintType.hpp"
#include "Constraint.hpp"
#include "ConstraintConcept.hpp"

template <PuzzleIntrinsics puzzle>
struct AsteriskSudokuConstraint : public Constraint<AsteriskSudokuConstraint<puzzle>, puzzle> {
public:
  constexpr AsteriskSudokuConstraint()
      : Constraint<AsteriskSudokuConstraint<puzzle>, puzzle>(
            ConstraintType::ASTERISK,
            "Asterisk",
            "The nine cells forming an asterisk contain all the digits exactly once.") {
    static_assert(ConstraintConcept<AsteriskSudokuConstraint, puzzle>,
                  "AsteriskSudokuConstraint does not satisfy ConstraintConcepFt");
  };

  constexpr static bool supportsPuzzle() {
    return puzzle.isSudoku();
  }

  constexpr static Option primaryOption(uint32_t row, uint32_t column, uint32_t digit) {
    constexpr std::array<std::pair<Index, Index>, 9> cells = {
        std::make_pair(1, 4), {2, 2}, {2, 6}, {4, 1}, {4, 4}, {4, 7}, {6, 2}, {6, 6}, {7, 4}};
    if (std::ranges::contains(cells, std::make_pair(row, column))) {
      return Option{static_cast<OptionId>(digit - 1)};
    }
    return {};
  }

  constexpr static Option
  secondaryOption([[maybe_unused]] uint32_t row, [[maybe_unused]] uint32_t column, [[maybe_unused]] uint32_t digit) {
    return {};
  }
};