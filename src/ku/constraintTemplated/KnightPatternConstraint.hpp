#pragma once

#include "ConstraintMacros.hpp"
#include "PatternConstraint.hpp"
#include "utilities/Patterns.hpp"

GENERATE_CONSTRAINT_TRAITS(KnightPatternConstraint, 0, Patterns::Knight.size());

template <PuzzleIntrinsics puzzle>
struct KnightPatternConstraint : public PatternConstraint<KnightPatternConstraint<puzzle>,
                                                          puzzle,
                                                          Patterns::Knight.size(),
                                                          Patterns::Knight,
                                                          false> {

  constexpr KnightPatternConstraint()
      : PatternConstraint<KnightPatternConstraint<puzzle>, puzzle, Patterns::Knight.size(), Patterns::Knight, false>(
            ConstraintType::KNIGHT_PATTERN,
            "Anti-Knight",
            "A digit cannot reappear in its neighborhood defined by direct Knight moves in Chess.") {
    CONSTRAINT_CONCEPT_ASSERT(KnightPatternConstraint, puzzle);
  }
};