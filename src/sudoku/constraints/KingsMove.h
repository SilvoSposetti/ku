#pragma once

#include "AbstractConstraint.h"

class KingsMove : public AbstractConstraint {
public:
  KingsMove() = default;

  virtual ConstraintType getType() const override;

  virtual std::string getName() const override;

  virtual std::string getDescription() const override;

  virtual std::string getSvgGroup() const override;

  virtual bool satisfy(const std::vector<std::vector<Sudo>>& board) const override;

  virtual int32_t getDLXConstraintColumnsAmount() const override;

  virtual bool getDLXConstraint(Sudo digit, int32_t i, int32_t j, const int32_t columnId) const override;

private:
  /**
   * Computes the list of all the valid index pairs of a given cell
   * @param rowIndex A cell's row index in the grid
   * @param columnIndex A cell's column index in the grid
   * @return The list of valid neighbors of that cell
   */
  static std::vector<std::pair<int32_t, int32_t>> getNeighbors(int32_t rowIndex, int32_t columnIndex);

  static std::vector<int32_t> getNeighboringBoxIds(int32_t rowIndex, int32_t columnIndex);
};