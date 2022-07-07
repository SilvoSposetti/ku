#pragma once

#include "../../utilities/Utilities.h"
#include "../SvgUtilities.h"

/** Base class for all constraints.
 */
class AbstractConstraint {
public:
  AbstractConstraint() = default;

  /** Defines the type of this constraint
   * @return The enum defining this constraint
   */
  virtual ConstraintType getType() const = 0;

  /** Defines the name of the constraint
   * @return The name
   */
  virtual std::string getName() const = 0;

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
  virtual bool satisfy(const std::vector<std::vector<Sudo>>& board) const = 0;

  /** Defines the amount of columns that this constraint prescribes for DLX
   * @return The amount of columns required
   */
  virtual int32_t getDlxConstraintColumnsAmount() const = 0;

  /** Defines the type of column according to its ID
   * @param columnId The ID of the column
   * @return Whether the column is secondary
   */
  virtual bool isColumnPrimary(int32_t columnId) const {
    return true;
  };

  /** Defines which cells of a column should have a 1 (true) or a 0 (false) for DLX matrix creation
   * @param digit The digit considered
   * @param i The board row considered
   * @param j The column row considered
   * @param columnId The DLX-matrix column considered
   * @return Whether the specific cell defined by the inputs should be a 0 or a 1 for DLX
   */
  virtual bool getDlxConstraint(Sudo digit, int32_t i, int32_t j, const int32_t columnId) const = 0;
};
