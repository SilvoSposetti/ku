#pragma once

#include "AbstractConstraint.h"

class AntiKingTorus : public AbstractConstraint {
public:
  AntiKingTorus();

  virtual ConstraintType getType() const override;

  virtual std::string getDescription() const override;

  virtual bool isColumnPrimary(int32_t columnId) const override;

  virtual std::string getSvgGroup() const override;

  virtual bool satisfy(const std::vector<std::vector<Sudo::Digit>>& board) const override;

  virtual int32_t getDlxConstraintColumnsAmount() const override;

  virtual bool getDlxConstraint(Sudo::Digit digit, int32_t i, int32_t j, const int32_t columnId) const override;

private:
  /** The pattern for the dashVector, couples the middle cell (0, 0) with those defined by the pattern
   */
  const std::set<std::pair<int32_t, int32_t>> pattern;

  /** Contains all possible dashes (point pairs) for the Anti-King's constraint
   */
  const std::vector<std::pair<std::pair<int32_t, int32_t>, std::pair<int32_t, int32_t>>> dashVector;
};