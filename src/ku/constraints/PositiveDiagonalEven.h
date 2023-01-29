#pragma once

#include "PositiveDiagonal.h"

class PositiveDiagonalEven : public PositiveDiagonal {
public:
  PositiveDiagonalEven() = default;

  virtual ConstraintType getType() const override;

  virtual std::string getDescription() const override;

  virtual std::string getSvgGroup() const override;

  virtual bool satisfy(const std::vector<std::vector<Sudo>>& board) const override;

  virtual int32_t getDlxConstraintColumnsAmount() const override;

  virtual bool getDlxConstraint(Sudo digit, int32_t i, int32_t j, const int32_t columnId) const override;
};