#pragma once

#include "SvgRect.hpp"
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

  static constexpr bool supportsPuzzle() {
    return puzzle.isSudoku();
  }

  static constexpr Option<ConstraintTraits<AsteriskSudokuConstraint<puzzle>>::primarySize>
  primaryOption(uint32_t row, uint32_t column, uint32_t digit) {
    if (std::ranges::any_of(cells, [&](const auto& pair) { return pair.first == row && pair.second == column; })) {
      return {static_cast<OptionId>(digit - 1)};
    }
    return {};
  }

  static constexpr Option<ConstraintTraits<AsteriskSudokuConstraint<puzzle>>::secondarySize>
  secondaryOption([[maybe_unused]] uint32_t row, [[maybe_unused]] uint32_t column, [[maybe_unused]] uint32_t digit) {
    return {};
  }

  virtual std::unique_ptr<SvgGroup>
  getSvgGroup(const DrawingOptionsTemplated<puzzle.getPuzzleSpace()>& options) const override {
    auto group = std::make_unique<SvgGroup>(this->getName(), "transparent", "black", options.mediumLine);
    for (const auto& [i, j] : cells) {
      const double topLeftX = i * options.cellSize;
      const double topLeftY = j * options.cellSize;
      group->add(std::make_unique<SvgRect>(topLeftX, topLeftY, options.cellSize, options.cellSize));
    }
    return group;
  }

private:
  /// The cells that are part of the asterisk pattern
  inline static constexpr std::array<std::pair<Index, Index>, 9> cells = {
      std::make_pair(1, 4), {2, 2}, {2, 6}, {4, 1}, {4, 4}, {4, 7}, {6, 2}, {6, 6}, {7, 4}};
};
