#pragma once

#include "AbstractConstraint.hpp"

#include <set>

class AntiKingTorus : public AbstractConstraint {
public:
  AntiKingTorus();

  virtual std::unique_ptr<SvgGroup> getSvgGroup(const DrawingOptions& options) const override;

  virtual bool satisfy(const std::vector<std::vector<Sudo::Digit>>& board) const override;

  virtual bool computeConstraint(Sudo::Digit digit, int32_t i, int32_t j, const int32_t columnId) const;

protected:
  virtual int32_t definePrimaryItemsAmount() const override;
  virtual std::vector<std::vector<int32_t>> definePrimaryItems() const override;
  virtual int32_t defineSecondaryItemsAmount() const override;
  virtual std::vector<std::vector<int32_t>> defineSecondaryItems() const override;

private:
  /** The pattern for the dashVector, couples the middle cell (0, 0) with those defined by the pattern
   */
  const std::set<std::pair<int32_t, int32_t>> pattern;

  /** Contains all possible dashes (point pairs) for the Anti-King's constraint
   */
  const std::vector<std::pair<std::pair<int32_t, int32_t>, std::pair<int32_t, int32_t>>> dashVector;
};