#pragma once

#include "AbstractConstraint.h"

class Asterisk : public AbstractConstraint {
public:
  Asterisk();

  virtual ConstraintType getType() const override;

  virtual std::string getDescription() const override;

  virtual std::string getSvgGroup() const override;

  virtual bool satisfy(const std::vector<std::vector<Sudo::Digit>>& board) const override;

  virtual int32_t getItemsAmount() const override;

  virtual bool computeConstraint(Sudo::Digit digit, int32_t i, int32_t j, const int32_t columnId) const override;

private:
  /** The cells that form the Asterisk's "box"
   */
  std::set<std::pair<int32_t, int32_t>> cells;
};