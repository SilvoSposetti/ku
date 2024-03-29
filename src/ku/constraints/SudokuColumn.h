#pragma once

#include "AbstractConstraint.h"

class SudokuColumn : public AbstractConstraint {
public:
  SudokuColumn() = default;

  virtual ConstraintType getType() const override;

  virtual std::string getDescription() const override;

  virtual std::string getSvgGroup() const override;

  virtual bool satisfy(const std::vector<std::vector<Sudo::Digit>>& board) const override;

  virtual int32_t getDlxConstraintColumnsAmount() const override;

  virtual bool getDlxConstraint(Sudo::Digit digit, int32_t i, int32_t j, const int32_t columnId) const override;
};
