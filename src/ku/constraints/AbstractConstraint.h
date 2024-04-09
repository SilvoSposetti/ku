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
   * @param type The type
   * @param name The name
   * @param description The description
   */
  AbstractConstraint(ConstraintType type, const std::string& name, const std::string& description);

  /** Virtual default destructor, otherwise child classes' destructors are not called
   */
  virtual ~AbstractConstraint() = default;

  /** Function that initializes the constraint with actual data, uses internal pure virtual function implementations.
   */
  void initialize();

  /** Retrieves the type of the constraint
   * @return The type
   */
  const ConstraintType getType() const;

  /** Retrieves the name of the constraint
   * @return The name
   */
  const std::string& getName() const;

  /** Retrieves the description of the constraint
   * @return The description
   */
  const std::string& getDescription() const;

  /** Retrieves the amount of primary items for the constraint
   * @return The amount of primary items
   */
  virtual int32_t getPrimaryItemsAmount() const;

  /** Retrieves the item indices of all the primary items for options in canonical order
   * @return The list of primary item indices
   */
  virtual const std::vector<std::vector<int32_t>>& getPrimaryItems() const;

  /** Retrieves the amount of secondary items for the constraint
   * @return The amount of secondary items
   */
  virtual int32_t getSecondaryItemsAmount() const;

  /** Retrieves the item indices of all the secondary items for options in canonical order
   * @return The list of secondary item indices
   */
  virtual const std::vector<std::vector<int32_t>>& getSecondaryItems() const;

  /** Defines how the constraint should be drawn on the board
   * @return The svg group that will be drawn
   */
  virtual std::string getSvgGroup() const = 0;

  /** Defines whether a given board satisfies the constraint
   * @param board A given board to check for validity
   * @return Whether the board satisfies this constraint
   */
  virtual bool satisfy(const std::vector<std::vector<Sudo::Digit>>& board) const = 0;

protected:
  /** Helper to retrieve the canonical ordering of options
   * @return The list of (rowIndex, columnIndex, digit).
   */
  static std::vector<std::tuple<int32_t, int32_t, Sudo::Digit>> optionsOrdered();

  /** Defines the amount of primary items for the constraint
   * @return The amount of primary items
   */
  virtual int32_t definePrimaryItemsAmount() const = 0;

  /** Defines the item indices of all the primary items for options in canonical order
   * @return The list of primary item indices
   */
  virtual std::vector<std::vector<int32_t>> definePrimaryItems() const = 0;

  /** Defines the amount of secondary items for the constraint
   * @return The amount of secondary items
   */
  virtual int32_t defineSecondaryItemsAmount() const = 0;

  /** Defines the item indices of all the secondary items for options in canonical order
   * @return The list of secondary item indices
   */
  virtual std::vector<std::vector<int32_t>> defineSecondaryItems() const = 0;

private:
  /** The type of constraint
   */
  const ConstraintType type = ConstraintType::NONE;
  /** The name of the constraint
   */
  const std::string name;
  /** The description of the constraint
   */
  const std::string description;

  /** The amount of primary items
   */
  int32_t primaryItemsAmount;

  /** The amount of secondary items
   */
  int32_t secondaryItemsAmount;

  /** The primary items
   */
  std::vector<std::vector<int32_t>> primaryItems;
  
  /** The secondary items
   */
  std::vector<std::vector<int32_t>> secondaryItems;

  /** Whether the constraint has been initialized
   */
  bool isInitialized = false;
};
