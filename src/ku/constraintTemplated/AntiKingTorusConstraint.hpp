#pragma once

#include "Constraint.hpp"
#include "ConstraintMacros.hpp"

GENERATE_CONSTRAINT_TRAITS(AntiKingTorusConstraint, 0, 8);

template <PuzzleIntrinsics puzzle>
struct AntiKingTorusConstraint : public Constraint<AntiKingTorusConstraint<puzzle>, puzzle> {
public:
  constexpr AntiKingTorusConstraint()
      : Constraint<AntiKingTorusConstraint<puzzle>, puzzle>(ConstraintType::ANTI_KING_TORUS,
                                                            "Anti-King-Torus",
                                                            "A digit cannot reappear in its Moore neighborhood. This "
                                                            "restriction also wraps around the edges of the puzzle.") {
    CONSTRAINT_CONCEPT_ASSERT(AntiKingTorusConstraint, puzzle);
  };

  static constexpr bool supportsPuzzle() {
    return puzzle.digits.size() >= 9;
  }

  static constexpr Option<ConstraintTraits<AntiKingTorusConstraint<puzzle>>::primarySize>
  primaryOption([[maybe_unused]] uint32_t row, [[maybe_unused]] uint32_t column, [[maybe_unused]] uint32_t digit) {
    return {};
  }

  static constexpr Option<ConstraintTraits<AntiKingTorusConstraint<puzzle>>::secondarySize>
  secondaryOption(uint32_t row, uint32_t column, uint32_t digit) {

    constexpr std::size_t count = 4;
    constexpr std::array<std::pair<uint32_t, uint32_t>, count> patternOffsets = {
        std::make_pair(1, 0), {1, -1}, {0, -1}, {-1, -1}};
    auto option = Option<count * 2>();

    // Set this cell 4 times, the times that other cells will find it in their pattern
    const auto cellId = puzzle.computeCellId(row, column);
    if (cellId.has_value()) {
      for (std::size_t i = 0; i < count; i++) {
        option.pushBack(static_cast<OptionId>(cellId.value() * count + i) * puzzle.digits.size() + (digit - 1));
      }
    }

    // Set the four cells constructed from the pattern for the current one
    std::size_t id = 0;
    for (const auto& offset : patternOffsets) {
      const auto location = puzzle.computeNeighborTorus(row, column, offset.first, offset.second);
      if (location.has_value()) {
        const auto neighborId = puzzle.computeCellId(location.value().first, location.value().second);
        if (neighborId.has_value()) {
          option.pushBack(
              static_cast<OptionId>((neighborId.value() * count + id++) * puzzle.digits.size() + (digit - 1)));
        }
      }
    }

    std::ranges::sort(option);
    return option;
  }
};