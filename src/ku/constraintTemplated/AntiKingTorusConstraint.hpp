#pragma once

#include "Constraint.hpp"
#include "ConstraintMacros.hpp"
#include "utilities/NeighborPatternUtilities.hpp"

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
    return NeighborPatternUtilities::computePatternOption<
        puzzle,
        pattern.size(),
        ConstraintTraits<AntiKingTorusConstraint<puzzle>>::secondarySize,
        true>(row, column, digit, pattern);
  }

private:
  /// The pattern used to generate the options
  static constexpr std::array<std::pair<int32_t, int32_t>, 8> pattern = {
      std::make_pair(0, 1), {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}};
};