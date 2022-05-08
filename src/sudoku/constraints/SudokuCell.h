#pragma once

#include "AbstractConstraint.h"

class SudokuCell : public AbstractConstraint {
public:
  SudokuCell() = default;

  virtual ConstraintType getType() const override;

  virtual std::string getName() const override;

  virtual std::string getDescription() const override;

  virtual std::string getSvgGroup() const override;

  virtual bool satisfy(const std::vector<std::vector<Sudo>>& board) const override;

  virtual int32_t getDLXConstraintColumnsAmount() const override;

  virtual bool getDLXConstraint(Sudo digit, int32_t i, int32_t j, const int32_t columnId) const override;
};