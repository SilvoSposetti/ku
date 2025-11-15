#pragma once

#include "Constraint.hpp"
#include "ConstraintMacros.hpp"
#include "utilities/NeighborPatternUtilities.hpp"

GENERATE_CONSTRAINT_TRAITS(AntiKingConstraint, 0, 8);

template <PuzzleIntrinsics puzzle>
struct AntiKingConstraint : public Constraint<AntiKingConstraint<puzzle>, puzzle> {
public:
  constexpr AntiKingConstraint()
      : Constraint<AntiKingConstraint<puzzle>, puzzle>(
            ConstraintType::ANTI_KING,
            "Anti-King",
            "A digit cannot reappear in its neighborhood defined by direct King moves in Chess") {
    CONSTRAINT_CONCEPT_ASSERT(AntiKingConstraint, puzzle);
  };

  static constexpr bool supportsPuzzle() {
    return puzzle.digits.size() >= pattern.size() + 1;
  }

  static constexpr Option<ConstraintTraits<AntiKingConstraint<puzzle>>::primarySize>
  primaryOption([[maybe_unused]] uint32_t row, [[maybe_unused]] uint32_t column, [[maybe_unused]] uint32_t digit) {
    return {};
  }

  static constexpr Option<ConstraintTraits<AntiKingConstraint<puzzle>>::secondarySize>
  secondaryOption(uint32_t row, uint32_t column, uint32_t digit) {
    return NeighborPatternUtilities::computePatternOption<
        puzzle,
        pattern.size(),
        ConstraintTraits<AntiKingConstraint<puzzle>>::secondarySize,
        false>(row, column, digit, pattern);
  }

private:
  /// The pattern used to generate the options
  static constexpr std::array<std::pair<int32_t, int32_t>, 8> pattern = {
      std::make_pair(0, 1), {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}};
};