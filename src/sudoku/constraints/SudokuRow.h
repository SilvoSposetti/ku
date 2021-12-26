#pragma once

#include "AbstractConstraint.h"

class SudokuRow: public AbstractConstraint {
public:
    SudokuRow() = default;

    virtual ConstraintType getType() override;

    virtual std::string getName() override;

    virtual std::string getDescription() override;

    virtual std::string getSvgGroup() override;
};
