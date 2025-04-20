#pragma once

#include "../constraints/ConstraintType.hpp"
#include "../puzzles/ItemsList.hpp"
#include "../puzzles/PuzzleIntrinsics.hpp"

#include <string_view>

/** Base class for all constraints.
 */
template <typename ConcreteConstraint, PuzzleIntrinsics puzzle>
struct Constraint {

  /** Constructor
   * @param type The type
   * @param name The name
   * @param description The description
   */
  constexpr Constraint(ConstraintType type, const std::string_view& name, const std::string_view& description)
      : type(type)
      , name(name)
      , description(description)
      , primaryItems(ConcreteConstraint::createPrimaryItems())
      , secondaryItems(ConcreteConstraint::createSecondaryItems()) {};

  /** The type of constraint
   */
  const ConstraintType type = ConstraintType::NONE;
  /** The name of the constraint
   */
  const std::string_view name;
  /** The description of the constraint
   */
  const std::string_view description;

  /** The primary items
   */
  const ItemsList<puzzle> primaryItems;

  /** The secondary items
   */
  const ItemsList<puzzle> secondaryItems;
};
