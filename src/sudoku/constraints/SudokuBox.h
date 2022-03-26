#pragma once

#include "AbstractConstraint.h"

class SudokuBox : public AbstractConstraint {
public:
  SudokuBox() = default;

  virtual ConstraintType getType() const override;

  virtual std::string getName() const override;

  virtual std::string getDescription() const override;

  virtual std::string getSvgGroup() const override;

  virtual bool validatePlacement(const Sudo digit,
                                 const int32_t rowIndex,
                                 const int32_t columnIndex,
                                 const std::vector<std::vector<Sudo>> board) const override;

  virtual bool satisfy(std::vector<std::vector<Sudo>> board) const override;

  virtual int32_t getDLXConstraintColumnsAmount() const override;

  virtual bool getDLXConstraint(const int32_t boardRow,
                                const int32_t boardColumn,
                                const int32_t columnId,
                                const Sudo possibleDigit,
                                const Sudo actualDigit) const override;

private:
  /**
   * Computes the "box" index of a cell, given its rowIndex and columnIndex in the grid
   * @param rowIndex A cell's row index in the grid
   * @param columnIndex A cell's column index in the grid
   * @return The "box" index for that cell
   */
  static int32_t getBoxIndex(const int32_t rowIndex, const int32_t columnIndex);

  /**
   * Creates all [row][column] index pairs for the cells of every box
   * @return A vector of vectors of index pairs. Outer vector references each box, inner vector contains all index
   * pairs for that box
   */
  static std::vector<std::vector<std::pair<int32_t, int32_t>>> getBoxIndices();
};
