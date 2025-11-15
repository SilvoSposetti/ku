#pragma once

#include "Constraint.hpp"
#include "ConstraintMacros.hpp"
#include "utilities/NeighborPatternUtilities.hpp"

GENERATE_CONSTRAINT_TRAITS(AntiKnightTorusConstraint, 0, 8);

template <PuzzleIntrinsics puzzle>
struct AntiKnightTorusConstraint : public Constraint<AntiKnightTorusConstraint<puzzle>, puzzle> {
public:
  constexpr AntiKnightTorusConstraint()
      : Constraint<AntiKnightTorusConstraint<puzzle>, puzzle>(
            ConstraintType::ANTI_KNIGHT_TORUS,
            "Anti-Knight-Torus",
            "A digit cannot reappear in its neighborhood defined by direct Knights moves in Chess. This "
            "restriction also wraps around the edges of the puzzle.") {
    CONSTRAINT_CONCEPT_ASSERT(AntiKnightTorusConstraint, puzzle);
  };

  static constexpr bool supportsPuzzle() {
    return puzzle.digits.size() >= 9;
  }

  static constexpr Option<ConstraintTraits<AntiKnightTorusConstraint<puzzle>>::primarySize>
  primaryOption([[maybe_unused]] uint32_t row, [[maybe_unused]] uint32_t column, [[maybe_unused]] uint32_t digit) {
    return {};
  }

  static constexpr Option<ConstraintTraits<AntiKnightTorusConstraint<puzzle>>::secondarySize>
  secondaryOption(uint32_t row, uint32_t column, uint32_t digit) {
    return NeighborPatternUtilities::computePatternOption<
        puzzle,
        pattern.size(),
        ConstraintTraits<AntiKnightTorusConstraint<puzzle>>::secondarySize,
        true>(row, column, digit, pattern);
  }

private:
  /// The pattern used to generate the options
  static constexpr std::array<std::pair<int32_t, int32_t>, 8> pattern = {
      std::make_pair(2, -1), {1, -2}, {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}, {1, 2}, {2, 1}};
};