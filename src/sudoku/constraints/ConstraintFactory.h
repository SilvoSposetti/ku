#pragma once

#include "AbstractConstraint.h"
#include <memory>

class ConstraintFactory {
public:

    ConstraintFactory() = delete;

    static std::unique_ptr<AbstractConstraint> makeConstraint(ConstraintType constraintType);
};

