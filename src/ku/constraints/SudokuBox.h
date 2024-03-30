#pragma once

#include "AbstractConstraint.h"

class SudokuBox : public AbstractConstraint {
public:
  SudokuBox();

  virtual std::string getSvgGroup() const override;

  virtual bool satisfy(const std::vector<std::vector<Sudo::Digit>>& board) const override;

  virtual int32_t getItemsAmount() const override;

  virtual bool computeConstraint(Sudo::Digit digit, int32_t i, int32_t j, const int32_t itemId) const override;

private:
  /** Computes the "box" index of a cell, given its rowIndex and columnIndex in the grid
   * @param rowIndex A cell's row index in the grid
   * @param columnIndex A cell's column index in the grid
   * @return The "box" index for that cell
   */
  static int32_t getBoxId(const int32_t rowIndex, const int32_t columnIndex);

  /** Creates all [row][column] index pairs for the cells of every box
   * @return A vector of vectors of index pairs. Outer vector references each box, inner vector contains all index
   * pairs for that box
   */
  static std::vector<std::vector<std::pair<int32_t, int32_t>>> getBoxIndices();
};
