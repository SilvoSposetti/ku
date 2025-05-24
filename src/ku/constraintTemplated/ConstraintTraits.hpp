#pragma once
#include "../puzzles/OptionId.hpp"

/** The traits of a constraint
 */
struct ConstraintTraits {
  /** Constructor
   */
  constexpr ConstraintTraits(OptionId primaryOptionsSize, OptionId secondaryOptionsSize)
      : primarySize(primaryOptionsSize)
      , secondarySize(secondaryOptionsSize) {}

  /** The capacity of all primary options for a constraint
   */
  const OptionId primarySize = 0;

  /** The capacity of all secondary options for a constraint
   */
  const OptionId secondarySize = 0;
};
