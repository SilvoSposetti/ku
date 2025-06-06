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
    return puzzle.digits.size() > 8;
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
    std::array<OptionId, count * 2> option = std::array<OptionId, count * 2>();

    uint32_t neighborId = 0;
    std::ranges::transform(patternOffsets, option.begin(), [&]([[maybe_unused]] const auto& offset) -> OptionId {
      const auto cellIdOptional = puzzle.computeCellId(row, column);
      if (cellIdOptional.has_value()) {
        return static_cast<OptionId>((cellIdOptional.value() * count + neighborId++) * puzzle.digits.size() +
                                     (digit - 1));
      }
      return OptionId(0);
    });

    neighborId = 0;
    std::ranges::transform(patternOffsets, option.begin() + count, [&](const auto& offset) -> OptionId {
      const auto location = puzzle.computeNeighborTorus(row, column, offset.first, offset.second);
      const auto neighborIdOptional = puzzle.computeCellId(location.first, location.second);
      if (neighborIdOptional.has_value()) {
        return static_cast<OptionId>((neighborIdOptional.value() * count + neighborId++) * puzzle.digits.size() +
                                     (digit - 1));
      }
      return OptionId(0);
    });

    std::ranges::sort(option);
    return {option[0], option[1], option[2], option[3], option[4], option[5], option[6], option[7]};
  }
};