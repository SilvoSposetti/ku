#pragma once

#include "ConstraintMacros.hpp"
#include "PatternConstraint.hpp"
#include "utilities/Patterns.hpp"

GENERATE_CONSTRAINT_TRAITS(KingPatternConstraint, 0, Patterns::Moore.size());

template <PuzzleIntrinsics puzzle>
struct KingPatternConstraint
    : public PatternConstraint<KingPatternConstraint<puzzle>, puzzle, Patterns::Moore.size(), Patterns::Moore, false> {

  constexpr KingPatternConstraint()
      : PatternConstraint<KingPatternConstraint<puzzle>, puzzle, Patterns::Moore.size(), Patterns::Moore, false>(
            ConstraintType::KING_PATTERN,
            "Anti-King",
            "A digit cannot reappear in its neighborhood defined by direct King moves in Chess.") {
    CONSTRAINT_CONCEPT_ASSERT(KingPatternConstraint, puzzle);
  }
};
