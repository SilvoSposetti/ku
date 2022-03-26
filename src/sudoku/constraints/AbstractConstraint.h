#pragma once

#include "../../utilities/Utilities.h"
#include "../SvgUtilities.h"

class AbstractConstraint {
public:
  AbstractConstraint() = default;

  virtual ConstraintType getType() const = 0;

  virtual std::string getName() const = 0;

  virtual std::string getDescription() const = 0;

  virtual std::string getSvgGroup() const = 0;

  virtual bool validatePlacement(Sudo digit,
                                 int32_t rowIndex,
                                 int32_t columnIndex,
                                 const std::vector<std::vector<Sudo>>& board) const = 0;

  virtual bool satisfy(const std::vector<std::vector<Sudo>>& board) const = 0;

  virtual int32_t getDLXConstraintColumnsAmount() const = 0;

  virtual bool getDLXConstraint(Sudo digit, int32_t i, int32_t j, const int32_t columnId) const = 0;
};
