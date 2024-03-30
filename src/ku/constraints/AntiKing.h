#pragma once

#include "AbstractConstraint.h"

class AntiKing : public AbstractConstraint {
public:
  AntiKing();

  virtual bool isItemPrimary(int32_t columnId) const override;

  virtual std::string getSvgGroup() const override;

  virtual bool satisfy(const std::vector<std::vector<Sudo::Digit>>& board) const override;

  virtual int32_t getItemsAmount() const override;

  virtual bool computeConstraint(Sudo::Digit digit, int32_t i, int32_t j, const int32_t columnId) const override;

private:
  /** Contains all possible dashes (point pairs) for the Anti-King's constraint
   */
  const std::vector<std::pair<std::pair<int32_t, int32_t>, std::pair<int32_t, int32_t>>> dashVector;
};