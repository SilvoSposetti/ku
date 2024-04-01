#pragma once

#include "../SvgUtilities.h"
#include "../utilities/IdPacking.h"
#include "ConstraintType.h"

#include <cmath>
#include <iostream>
#include <set>

/** Base class for all constraints.
 */
class AbstractConstraint {
public:
  /** Constructor
   * @param type The type of constraint
   * @param name The name of the constraint
   * @param description The description of the constraint
   */
  AbstractConstraint(ConstraintType type, const std::string& name, const std::string& description);

  /** Virtual default destructor, otherwise child classes' destructors are not called
   */
  virtual ~AbstractConstraint() = default;

  /** Defines the type of the constraint
   * @return The type
   */
  const ConstraintType getType() const;

  /** Defines the name of the constraint
   * @return The name
   */
  const std::string& getName() const;

  /** Defines the description of the constraint
   * @return The description
   */
  const std::string& getDescription() const;

  /** Defines how the constraint should be drawn on the board
   * @return The svg group that will be drawn
   */
  virtual std::string getSvgGroup() const = 0;

  /** Defines whether a given board satisfies the constraint
   * @param board A given board to check for validity
   * @return Whether the board satisfies this constraint
   */
  virtual bool satisfy(const std::vector<std::vector<Sudo::Digit>>& board) const = 0;

  /** Defines the amount of primary items for the constraint
   * @return The amount of primary items
   */
  virtual int32_t getPrimaryItemsAmount() const = 0;

  /** Defines the item indices of all the primary items for options in canonical order
   * @return The list of primary item indices
   */
  virtual std::vector<std::vector<int32_t>> getPrimaryItems() const = 0;

  /** Defines the amount of secondary items for the constraint
   * @return The amount of secondary items
   */
  virtual int32_t getSecondaryItemsAmount() const = 0;

  /** Defines the item indices of all the secondary items for options in canonical order
   * @return The list of secondary item indices
   */
  virtual std::vector<std::vector<int32_t>> getSecondaryItems() const = 0;

protected:
  /** Helper to retrieve the canonical ordering of options
   * @return The list of (rowIndex, columnIndex, digit).
   */
  static std::vector<std::tuple<int32_t, int32_t, Sudo::Digit>> optionsOrdered();

private:
  /** The type of constraint
   */
  const ConstraintType type;
  /** The name of the constraint
   */
  const std::string name;
  /** The description of the constraint
   */
  const std::string description;
};
