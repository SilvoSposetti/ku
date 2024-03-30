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
  const std::string& getType() const;

  /** Defines the name of the constraint
   * @return The name
   */
  const std::string& getName() const;

  /** Defines the description of the constraint
   * @return The description
   */
  const std::string& getDescription() const;

  /** Retrieves a list of all the item IDs that are non-zero, for each possible row in the Algortithm X's matrix
   * @return A list in canonical ordering of all the rows, of all the non-zero item IDs
   */
  std::vector<std::vector<int32_t>> getDlxConstraints() const;

  /** Defines how the constraint should be drawn on the board
   * @return The svg group that will be drawn
   */
  virtual std::string getSvgGroup() const = 0;

  /** Defines whether a given board satisfies the constraint
   * @param board A given board to check for validity
   * @return Whether the board satisfies this constraint
   */
  virtual bool satisfy(const std::vector<std::vector<Sudo::Digit>>& board) const = 0;

  /** Defines the amount of items that this constraint prescribes for the Exact Cover problem
   * @return The amount of items
   */
  virtual int32_t getItemsAmount() const = 0;

  /** Defines the type of item
   * @param itemId The ID of the item
   * @return Whether the item is secondary
   */
  virtual bool isItemPrimary(int32_t itemId) const;

  /** Computes whether the constraint specifies only secondary items
   * @return Whether the constraint specifies only secondary itmes
   */
  bool isSecondaryItemsOnly() const;

protected:
  /** Defines which cells of a item should be set or not for the Algorithm X's matrix
   * @param digit The digit considered
   * @param i The board row considered
   * @param j The column row considered
   * @param itemId The item ID
   * @return Whether the cell defined by the inputs should be set Algorithm X's matrix
   */
  virtual bool computeConstraint(Sudo::Digit digit, int32_t i, int32_t j, int32_t itemId) const = 0;

  /** Computes and returns the set of point-point pairs that can be constructed for all cells in a grid according to the
   input pattern. The pattern describes single-cell pairs that need to be built with the central cell (0, 0).
   * @param pattern The pattern for the pairs
   * @param doTorus Whether the pattern should wrap around the board
   * @return The fully-constructed set of all cell pairs according to the pattern, without duplicates
   */
  static std::vector<std::pair<std::pair<int32_t, int32_t>, std::pair<int32_t, int32_t>>>
  createDashVector(std::set<std::pair<int32_t, int32_t>> pattern, bool doTorus);

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
