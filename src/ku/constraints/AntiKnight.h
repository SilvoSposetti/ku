#pragma once

#include "AbstractConstraint.h"

class AntiKnight : public AbstractConstraint {
public:
  AntiKnight();

  virtual ConstraintType getType() const override;

  virtual std::string getDescription() const override;

  virtual bool isColumnPrimary(int32_t columnId) const override;

  virtual std::string getSvgGroup() const override;

  virtual bool satisfy(const std::vector<std::vector<Sudo::Digit>>& board) const override;

  virtual int32_t getDlxConstraintColumnsAmount() const override;

  virtual bool getDlxConstraint(Sudo::Digit digit, int32_t i, int32_t j, const int32_t columnId) const override;

private:
  /** Contains all possible dashes (point pairs) for the Anti-King's constraint
   */
  const std::vector<std::pair<std::pair<int32_t, int32_t>, std::pair<int32_t, int32_t>>> dashVector;
};