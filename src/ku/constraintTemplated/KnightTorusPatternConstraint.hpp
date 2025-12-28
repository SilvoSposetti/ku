#pragma once

#include "ConstraintMacros.hpp"
#include "PatternConstraint.hpp"
#include "utilities/Patterns.hpp"

GENERATE_CONSTRAINT_TRAITS(KnightTorusPatternConstraint, 0, Patterns::Knight.size());

template <PuzzleIntrinsics puzzle>
struct KnightTorusPatternConstraint : public PatternConstraint<KnightTorusPatternConstraint<puzzle>,
                                                               puzzle,
                                                               Patterns::Knight.size(),
                                                               Patterns::Knight,
                                                               true> {

  constexpr KnightTorusPatternConstraint()
      : PatternConstraint<KnightTorusPatternConstraint<puzzle>,
                          puzzle,
                          Patterns::Knight.size(),
                          Patterns::Knight,
                          true>(ConstraintType::KNIGHT_TORUS_PATTERN,
                                "Anti-Knight-Torus",
                                "A digit cannot reappear in its neighborhood defined by direct Knight moves in Chess. "
                                "This restriction also wraps around the edges of the puzzle.") {
    CONSTRAINT_CONCEPT_ASSERT(KnightTorusPatternConstraint, puzzle);
  }
};
