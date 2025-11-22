#pragma once

#include "ConstraintMacros.hpp"
#include "PatternConstraint.hpp"
#include "utilities/Patterns.hpp"

GENERATE_CONSTRAINT_TRAITS(AntiKnightTorusConstraint, 0, Patterns::Knight.size());

template <PuzzleIntrinsics puzzle>
struct AntiKnightTorusConstraint : public PatternConstraint<AntiKnightTorusConstraint<puzzle>,
                                                            puzzle,
                                                            Patterns::Knight.size(),
                                                            Patterns::Knight,
                                                            true> {

  constexpr AntiKnightTorusConstraint()
      : PatternConstraint<AntiKnightTorusConstraint<puzzle>, puzzle, Patterns::Knight.size(), Patterns::Knight, true>(
            ConstraintType::ANTI_KNIGHT_TORUS,
            "Anti-Knight-Torus",
            "A digit cannot reappear in its neighborhood defined by direct Knights moves in Chess. This restriction "
            "also wraps around the edges of the puzzle.") {
    CONSTRAINT_CONCEPT_ASSERT(AntiKnightTorusConstraint, puzzle);
  }
};