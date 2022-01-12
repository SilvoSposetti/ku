#pragma once

#include "AbstractConstraint.h"

class SudokuRow : public AbstractConstraint {
public:
    SudokuRow() = default;

    virtual ConstraintType getType() const override;

    virtual std::string getName() const override;

    virtual std::string getDescription() const override;

    virtual std::string getSvgGroup() const override;

    virtual bool validatePlacement(const Sudo digit,
                                   const int8_t rowIndex,
                                   const int8_t columnIndex,
                                   const std::vector<std::vector<Sudo>> board) const override;

    virtual bool satisfy(std::vector<std::vector<Sudo>> board) const override;

    virtual int32_t getDLXConstraintColumnsAmount() const override;

    virtual bool getDLXConstraint(const int8_t boardRow,
                                  const int8_t boardColumn,
                                  const int32_t columnId,
                                  const Sudo possibleDigit,
                                  const Sudo actualDigit) const override;
};
