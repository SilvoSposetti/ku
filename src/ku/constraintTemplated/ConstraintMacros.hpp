#pragma once

#include "ConstraintConcept.hpp"
#include "ConstraintTraits.hpp"
#include "ConstraintTraitsConcept.hpp"

/** Macro to simplify the addition of the traits specialization for a constraint, which involves forward-declaring the
 * constraint as well.
 * @param CONSTRAINT_NAME The name of the constraint
 * @param PRIMARY_SIZE The capacity of the primary items that the constraint will generate
 * @param SECONDARY_SIZE The capacity of the secondary items that the constraint will generate
 */
#define GENERATE_CONSTRAINT_TRAITS(CONSTRAINT_NAME, PRIMARY_SIZE, SECONDARY_SIZE)                                      \
  template <PuzzleIntrinsics puzzle>                                                                                   \
  struct CONSTRAINT_NAME;                                                                                              \
  template <PuzzleIntrinsics puzzle>                                                                                   \
  struct ConstraintTraits<CONSTRAINT_NAME<puzzle>> {                                                                   \
    static constexpr OptionId primarySize = PRIMARY_SIZE;                                                              \
    static constexpr OptionId secondarySize = SECONDARY_SIZE;                                                          \
    static_assert(ConstraintTraitConcept<ConstraintTraits<CONSTRAINT_NAME<puzzle>>>,                                   \
                  #CONSTRAINT_NAME " does not satisfy ConstraintTraitConcept");                                        \
  }

#define CONSTRAINT_CONCEPT_ASSERT(CONSTRAINT_NAME, PUZZLE_INTRINISCS)                                                  \
  static_assert(ConstraintConcept<CONSTRAINT_NAME,                                                                     \
                                  PUZZLE_INTRINISCS,                                                                   \
                                  ConstraintTraits<CONSTRAINT_NAME>::primarySize,                                      \
                                  ConstraintTraits<CONSTRAINT_NAME>::secondarySize>,                                   \
                #CONSTRAINT_NAME " does not satisfy ConstraintConcept")
