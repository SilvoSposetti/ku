#pragma once

#include "AbstractConstraint.h"

class PositiveDiagonal : public AbstractConstraint {
public:
  PositiveDiagonal() = default;

  virtual ConstraintType getType() const override;

  virtual std::string getName() const override;

  virtual std::string getDescription() const override;

  virtual std::string getSvgGroup() const override;

  virtual bool validatePlacement(Sudo digit,
                                 int32_t rowIndex,
                                 int32_t columnIndex,
                                 const std::vector<std::vector<Sudo>>& board) const override;

  virtual bool satisfy(const std::vector<std::vector<Sudo>>& board) const override;

  virtual int32_t getDLXConstraintColumnsAmount() const override;

  virtual bool getDLXConstraint(Sudo digit, int32_t i, int32_t j, const int32_t columnId) const override;

private:
  /**
   * Generates all index pairs that identify the cells of the positive diagonal
   * @return The set of index pairs of the positive
   */
  static std::vector<std::pair<int32_t, int32_t>> getAllPositiveDiagonalIndices();

  /**
   * Generates all index pairs that identify the cells of the positive diagonal
   * @return The set of index pairs of the positive
   */
  static bool isOnPositiveDiagonal(int32_t i, int32_t j);
};