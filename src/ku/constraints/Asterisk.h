#pragma once

#include "AbstractConstraint.h"

#include <set>

class Asterisk : public AbstractConstraint {
public:
  Asterisk();

  virtual std::unique_ptr<Group> getSvgGroup(const DrawingOptions& options) const override;

  virtual bool satisfy(const std::vector<std::vector<Sudo::Digit>>& board) const override;

  virtual bool computeConstraint(Sudo::Digit digit, int32_t i, int32_t j, const int32_t columnId) const;

protected:
  virtual int32_t definePrimaryItemsAmount() const override;
  virtual std::vector<std::vector<int32_t>> definePrimaryItems() const override;
  virtual int32_t defineSecondaryItemsAmount() const override;
  virtual std::vector<std::vector<int32_t>> defineSecondaryItems() const override;

private:
  /** The cells that form the Asterisk's "box"
   */
  std::set<std::pair<int32_t, int32_t>> cells;
};