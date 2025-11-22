#pragma once

#include "ConstraintMacros.hpp"
#include "PatternConstraint.hpp"
#include "utilities/Patterns.hpp"

GENERATE_CONSTRAINT_TRAITS(AntiKingTorusConstraint, 0, Patterns::Moore.size());

template <PuzzleIntrinsics puzzle>
struct AntiKingTorusConstraint
    : public PatternConstraint<AntiKingTorusConstraint<puzzle>, puzzle, Patterns::Moore.size(), Patterns::Moore, true> {

  constexpr AntiKingTorusConstraint()
      : PatternConstraint<AntiKingTorusConstraint<puzzle>, puzzle, Patterns::Moore.size(), Patterns::Moore, true>(
            ConstraintType::ANTI_KING_TORUS,
            "Anti-King-Torus",
            "A digit cannot reappear in its Moore neighborhood. This restriction also wraps around "
            "the edges of the puzzle.") {
    CONSTRAINT_CONCEPT_ASSERT(AntiKingTorusConstraint, puzzle);
  }
};