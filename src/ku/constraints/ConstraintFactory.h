#pragma once

#include "AbstractConstraint.h"

#include <memory>

/** A simple factory pattern to create constraints
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
