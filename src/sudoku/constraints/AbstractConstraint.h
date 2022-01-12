#pragma once

#include "../../utilities/Utilities.h"

class AbstractConstraint {
public:
    AbstractConstraint() = default;

    virtual ConstraintType getType() const = 0;

    virtual std::string getName() const = 0;

    virtual std::string getDescription() const = 0;

    virtual std::string getSvgGroup() const = 0;

    virtual bool validatePlacement(const Sudo digit,
                                   const int8_t rowIndex,
                                   const int8_t columnIndex,
                                   const std::vector<std::vector<Sudo>> board) const = 0;

    virtual bool satisfy(std::vector<std::vector<Sudo>> board) const = 0;

    virtual int32_t getDLXConstraintColumnsAmount() const = 0;

    virtual bool getDLXConstraint(const int8_t boardRow,
                                  const int8_t boardColumn,
                                  const int32_t columnId,
                                  const Sudo possibleDigit,
                                  const Sudo actualDigit) const = 0;

};

