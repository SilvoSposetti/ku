#pragma once

#include "../puzzles/OptionId.hpp"
#include "ConstraintTraitsConcept.hpp"

/** The traits of a constraint. This class is used to externalize the specification of the maximum primary and secondary
 * option sizes out of a single constraint. This way, the definition of the sizes is well-known at compile-time in both
 * the derived constraint classes, as well as in their base CRTP class.
 * @tparam The constraint for which the traits will be used.
 */
template <typename ConcreteConstraint>
struct ConstraintTraits {
  /** The capacity of all primary options for a constraint
   */
  static constexpr OptionId primarySize = 0;

  /** The capacity of all secondary options for a constraint
   */
  static constexpr OptionId secondarySize = 0;
};

static_assert(ConstraintTraitConcept<ConstraintTraits<void>>,
              " ConstraintTraits base class does not satisfy ConstraintTraitConcept");