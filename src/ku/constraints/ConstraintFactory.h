#pragma once

#include "AbstractConstraint.h"

/** A simple factoy pattern to create constraints
 */
class ConstraintFactory {
public:
  ConstraintFactory() = delete;

  /** Factory method for creating constraints
   * @param constraintType The type of constraint that should be created
   * @return The constructed constraint
   */
  static std::unique_ptr<AbstractConstraint> makeConstraint(ConstraintType constraintType);
};
