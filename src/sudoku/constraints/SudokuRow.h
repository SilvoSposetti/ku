#pragma once

#include "AbstractConstraint.h"

class SudokuRow : public AbstractConstraint {
public:
    SudokuRow() = default;

    virtual ConstraintType getType() override;

    virtual std::string getName() override;

    virtual std::string getDescription() override;

    virtual std::string getSvgGroup() override;

    virtual bool validatePlacement(const Sudo digit,
                                   const int8_t rowIndex,
                                   const int8_t columnIndex,
                                   const std::vector<std::vector<Sudo>> board) override;

    virtual bool satisfy(std::vector<std::vector<Sudo>> board) override;
};
