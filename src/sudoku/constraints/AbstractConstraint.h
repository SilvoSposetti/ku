#pragma once

#include <iostream>
#include "../../utilities/Utilities.h"

class AbstractConstraint {
public:
    AbstractConstraint() = default;

    virtual ConstraintType getType() = 0;

    virtual std::string getName() = 0;

    virtual std::string getDescription() = 0;

    virtual std::string getSvgGroup() = 0;
};

