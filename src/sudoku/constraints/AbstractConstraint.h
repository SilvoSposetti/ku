#pragma once

#include "../../utilities/Utilities.h"

class AbstractConstraint {
public:
    AbstractConstraint() = default;

    virtual ConstraintType getType() = 0;

    virtual std::string getName() = 0;

    virtual std::string getDescription() = 0;

    virtual std::string getSvgGroup() = 0;

    virtual bool validatePlacement(const Sudo digit,
                                   const int8_t rowIndex,
                                   const int8_t columnIndex,
                                   const std::vector<std::vector<Sudo>> board) = 0;

    virtual bool satisfy(std::vector<std::vector<Sudo>> board) = 0;

};

