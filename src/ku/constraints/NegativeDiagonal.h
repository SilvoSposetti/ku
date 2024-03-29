#pragma once

#include "AbstractConstraint.h"

class NegativeDiagonal : public AbstractConstraint {
public:
  NegativeDiagonal() = default;

  virtual ConstraintType getType() const override;

  virtual std::string getDescription() const override;

  virtual std::string getSvgGroup() const override;

  virtual bool satisfy(const std::vector<std::vector<Sudo::Digit>>& board) const override;

  virtual int32_t getDlxConstraintColumnsAmount() const override;

  virtual bool getDlxConstraint(Sudo::Digit digit, int32_t i, int32_t j, const int32_t columnId) const override;

protected:
  /** Generates all index pairs that identify the cells of the negative diagonal
   * @return The set of index pairs of the negative diagonal
   */
  static std::vector<std::pair<int32_t, int32_t>> getAllNegativeDiagonalIndices();

  /** Generates all index pairs that identify the cells of the negative diagonal
   * @return The set of index pairs of the negative diagonal
   */
  static bool isOnNegativeDiagonal(int32_t i, int32_t j);
};