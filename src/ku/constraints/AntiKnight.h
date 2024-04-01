#pragma once

#include "AbstractConstraint.h"

class AntiKnight : public AbstractConstraint {
public:
  AntiKnight();

  virtual std::string getSvgGroup() const override;

  virtual bool satisfy(const std::vector<std::vector<Sudo::Digit>>& board) const override;

  virtual bool computeConstraint(Sudo::Digit digit, int32_t i, int32_t j, const int32_t columnId) const override;

  virtual int32_t getPrimaryItemsAmount() const override;
  virtual std::vector<std::vector<int32_t>> getPrimaryItems() const override;
  virtual int32_t getSecondaryItemsAmount() const override;
  virtual std::vector<std::vector<int32_t>> getSecondaryItems() const override;

private:
  /** Contains all possible dashes (point pairs) for the Anti-King's constraint
   */
  const std::vector<std::pair<std::pair<int32_t, int32_t>, std::pair<int32_t, int32_t>>> dashVector;
};