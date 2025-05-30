#pragma once

#include "../puzzles/OptionId.hpp"

#include <concepts>

/** Concept for a constraint trait.
 * @tparam ConstraintTrait The constraint trait to which the concept will apply.
 */
template <typename ConstraintTrait>
concept ConstraintTraitConcept = requires {
  // Ensure the members exist and are of the correct type.
  { ConstraintTrait::primarySize } -> std::same_as<const OptionId&>;
  { ConstraintTrait::secondarySize } -> std::same_as<const OptionId&>;

  // Force the members to be usable in constant expressions.
  [] {
    constexpr OptionId a = ConstraintTrait::primarySize;
    (void)a;
  }();
  [] {
    constexpr OptionId b = ConstraintTrait::secondarySize;
    (void)b;
  }();
};