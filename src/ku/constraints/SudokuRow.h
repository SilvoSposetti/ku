#pragma once

#include "AbstractConstraint.h"

class SudokuRow : public AbstractConstraint {
public:
  SudokuRow();

  virtual std::unique_ptr<Group> getSvgGroup(const DrawingOptions& options) const override;

  virtual bool satisfy(const std::vector<std::vector<Sudo::Digit>>& board) const override;

protected:
  virtual int32_t definePrimaryItemsAmount() const override;
  virtual std::vector<std::vector<int32_t>> definePrimaryItems() const override;
  virtual int32_t defineSecondaryItemsAmount() const override;
  virtual std::vector<std::vector<int32_t>> defineSecondaryItems() const override;
};
