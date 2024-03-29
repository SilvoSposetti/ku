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
  /** Default constructor
   */
  AbstractConstraint() = default;

  /** Virtual default destructor, otherwise child classes' destructors are not called
   */
  virtual ~AbstractConstraint() = default;

  /** Defines the name of the constraint
   * @return The name
   */
  std::string getName() const;

  /** Defines the type of this constraint
   * @return The enum defining this constraint
   */
  virtual ConstraintType getType() const = 0;

  /** Retrieves a list of all the columnIds that are non-zero, for each possible row in the Algortithm X's matrix
   * @return A list in canonical ordering of all the rows, of all the non-zero columnIds
   */
  std::vector<std::vector<int32_t>> getDlxConstraints() const;

  /** Defines the description of the constraint
   * @return The description
   */
  virtual std::string getDescription() const = 0;

  /** Defines how the constraint should be drawn on the board
   * @return The svg group that will be drawn
   */
  virtual std::string getSvgGroup() const = 0;

  /** Defines whether a given board satisfies the constraint
   * @param board A given board to check for validity
   * @return Whether the board satisfies this constraint
   */
  virtual bool satisfy(const std::vector<std::vector<Sudo::Digit>>& board) const = 0;

  /** Defines the amount of columns that this constraint prescribes for DLX
   * @return The amount of columns required
   */
  virtual int32_t getDlxConstraintColumnsAmount() const = 0;

  /** Defines the type of column according to its ID
   * @param columnId The ID of the column
   * @return Whether the column is secondary
   */
  virtual bool isColumnPrimary(int32_t columnId) const;

  /** Computes the result using getDlxConstraintColumnsAmount() and isColumnPrimary()
   * @return Whether the constraint specifies only secondary columns
   */
  bool isSecondaryColumnsOnly() const;

protected:
  /** Defines which cells of a column should have a 1 (true) or a 0 (false) for DLX matrix creation
   * @param digit The digit considered
   * @param i The board row considered
   * @param j The column row considered
   * @param columnId The DLX-matrix column considered
   * @return Whether the specific cell defined by the inputs should be a 0 or a 1 for DLX
   */
  virtual bool getDlxConstraint(Sudo::Digit digit, int32_t i, int32_t j, const int32_t columnId) const = 0;

  /** Computes and returns the set of point-point pairs that can be constructed for all cells in a grid according to the
   input pattern. The pattern describes single-cell pairs that need to be built with the central cell (0, 0).
   * @param pattern The pattern for the pairs
   * @param doTorus Whether the pattern should wrap around the board
   * @return The fully-constructed set of all cell pairs according to the pattern, without duplicates
   */
  static std::vector<std::pair<std::pair<int32_t, int32_t>, std::pair<int32_t, int32_t>>>
  createDashVector(std::set<std::pair<int32_t, int32_t>> pattern, bool doTorus);
};
