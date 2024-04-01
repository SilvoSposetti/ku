#pragma once

#include "AbstractConstraint.h"

class HyperSudoku : public AbstractConstraint {
public:
  HyperSudoku();

  virtual std::string getSvgGroup() const override;

  virtual bool satisfy(const std::vector<std::vector<Sudo::Digit>>& board) const override;

  virtual bool computeConstraint(Sudo::Digit digit, int32_t i, int32_t j, const int32_t itemId) const ;

  virtual int32_t getPrimaryItemsAmount() const override;
  virtual std::vector<std::vector<int32_t>> getPrimaryItems() const override;
  virtual int32_t getSecondaryItemsAmount() const override;
  virtual std::vector<std::vector<int32_t>> getSecondaryItems() const override;

private:
  /** Computes whether a cell identified by its row and column indices is within a certain box
   * @param rowIndex The row index of the cell
   * @param columnIndex The column index of the cell
   * @param boxId The box identifier, 0 is top-left, 1 is top-right, 2 is bottom-left, 3 is bottom-right
   * @return Whether the set of constraint is solvable
   */
  bool isCellInBox(int32_t rowIndex, int32_t columnIndex, int32_t boxId) const;

private:
  /** The four boxes' top-left corner cell indices
   */
  const std::vector<std::pair<int32_t, int32_t>> topLeftCorners;
};