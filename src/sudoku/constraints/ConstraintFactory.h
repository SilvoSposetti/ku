#pragma once

#include "AbstractConstraint.h"

class ConstraintFactory {
public:
  ConstraintFactory() = delete;

  static std::unique_ptr<AbstractConstraint> makeConstraint(ConstraintType constraintType);
};
