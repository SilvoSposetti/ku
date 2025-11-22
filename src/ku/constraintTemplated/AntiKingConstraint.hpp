#pragma once

#include "ConstraintMacros.hpp"
#include "PatternConstraint.hpp"
#include "utilities/Patterns.hpp"

GENERATE_CONSTRAINT_TRAITS(AntiKingConstraint, 0, Patterns::Moore.size());

template <PuzzleIntrinsics puzzle>
struct AntiKingConstraint
    : public PatternConstraint<AntiKingConstraint<puzzle>, puzzle, Patterns::Moore.size(), Patterns::Moore, false> {

  constexpr AntiKingConstraint()
      : PatternConstraint<AntiKingConstraint<puzzle>, puzzle, Patterns::Moore.size(), Patterns::Moore, false>(
            ConstraintType::ANTI_KING,
            "Anti-King",
            "A digit cannot reappear in its neighborhood defined by direct King moves in Chess.") {
    CONSTRAINT_CONCEPT_ASSERT(AntiKingConstraint, puzzle);
  }
};