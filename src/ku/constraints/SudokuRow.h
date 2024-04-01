#pragma once

#include "AbstractConstraint.h"

class SudokuRow : public AbstractConstraint {
public:
  SudokuRow();

  virtual std::string getSvgGroup() const override;

  virtual bool satisfy(const std::vector<std::vector<Sudo::Digit>>& board) const override;

  virtual int32_t getPrimaryItemsAmount() const override;
  virtual std::vector<std::vector<int32_t>> getPrimaryItems() const override;
  virtual int32_t getSecondaryItemsAmount() const override;
  virtual std::vector<std::vector<int32_t>> getSecondaryItems() const override;
};
