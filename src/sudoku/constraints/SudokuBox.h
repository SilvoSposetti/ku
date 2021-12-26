#pragma once

#include "AbstractConstraint.h"

class SudokuBox : public AbstractConstraint {
public:
    SudokuBox() = default;

    virtual ConstraintType getType() override;

    virtual std::string getName() override;

    virtual std::string getDescription() override;

    virtual std::string getSvgGroup() override;
};

