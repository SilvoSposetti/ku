#pragma once

#include "ConstraintMacros.hpp"
#include "PatternConstraint.hpp"
#include "utilities/Patterns.hpp"

GENERATE_CONSTRAINT_TRAITS(KingTorusPatternConstraint, 0, Patterns::Moore.size());

template <PuzzleIntrinsics puzzle>
struct KingTorusPatternConstraint : public PatternConstraint<KingTorusPatternConstraint<puzzle>,
                                                             puzzle,
                                                             Patterns::Moore.size(),
                                                             Patterns::Moore,
                                                             true> {

  constexpr KingTorusPatternConstraint()
      : PatternConstraint<KingTorusPatternConstraint<puzzle>, puzzle, Patterns::Moore.size(), Patterns::Moore, true>(
            ConstraintType::KING_TORUS_PATTERN,
            "Anti-King-Torus",
            "A digit cannot reappear in its Moore neighborhood. This restriction also wraps around the edges of the "
            "puzzle.") {
    CONSTRAINT_CONCEPT_ASSERT(KingTorusPatternConstraint, puzzle);
  }
};