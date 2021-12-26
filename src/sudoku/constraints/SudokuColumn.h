#pragma once

#include "AbstractConstraint.h"

class SudokuColumn : public AbstractConstraint {
public:
    SudokuColumn() = default;

    virtual ConstraintType getType() override;

    virtual std::string getName() override;

    virtual std::string getDescription() override;

    virtual std::string getSvgGroup() override;
};
