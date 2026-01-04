#pragma once

#include "Option.hpp"
#include "PuzzleIntrinsics.hpp"

#include <concepts>

/** Concept for a constraint. This is used to enforce certain functions to be available for the CRTP inheritance.
 * @tparam Constraint The constraint to which the concept will apply.
 * @tparam intrinsics The puzzle intrinsics for the specific constraint.
 * @tparam maxPrimaryOptionSize The size of primary items that the constraint is expected to produce.
 * @tparam maxSecondaryOptionsSize The size of secondary items that the constraint is expected to produce.
 */
template <typename Constraint,
          PuzzleIntrinsics intrinsics,
          OptionId maxPrimaryOptionSize,
          OptionId maxSecondaryOptionsSize>
concept ConstraintConcept =
    std::default_initializable<Constraint> && requires(uint32_t row, uint32_t column, uint32_t digit) {
      // Primary and secondary option-creating-functions.
      { Constraint::primaryOption(row, column, digit) } -> std::same_as<Option<maxPrimaryOptionSize>>;
      { Constraint::secondaryOption(row, column, digit) } -> std::same_as<Option<maxSecondaryOptionsSize>>;

      // Whether the constraint supports a specific puzzle.
      { Constraint::supportsPuzzle() } -> std::same_as<bool>;
    };
