#pragma once

#include "AbstractConstraint.h"

class PositiveDiagonalOdd : public AbstractConstraint {
public:
  PositiveDiagonalOdd();

  virtual std::string getSvgGroup() const override;

  virtual bool satisfy(const std::vector<std::vector<Sudo::Digit>>& board) const override;

  virtual bool computeConstraint(Sudo::Digit digit, int32_t i, int32_t j, const int32_t itemId) const ;

  virtual int32_t getPrimaryItemsAmount() const override;
  virtual std::vector<std::vector<int32_t>> getPrimaryItems() const override;
  virtual int32_t getSecondaryItemsAmount() const override;
  virtual std::vector<std::vector<int32_t>> getSecondaryItems() const override;
};