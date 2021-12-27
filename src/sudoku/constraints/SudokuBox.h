#pragma once

#include "AbstractConstraint.h"

class SudokuBox : public AbstractConstraint {
public:
    SudokuBox() = default;

    virtual ConstraintType getType() override;

    virtual std::string getName() override;

    virtual std::string getDescription() override;

    virtual std::string getSvgGroup() override;

    virtual bool validatePlacement(const Sudo digit,
                                   const int8_t rowIndex,
                                   const int8_t columnIndex,
                                   const std::vector<std::vector<Sudo>> board) override;

    virtual bool satisfy(std::vector<std::vector<Sudo>> board) override;

private:

    /**
     * Computes the "box" index of a cell, given its rowIndex and columnIndex in the grid
     * @param rowIndex A cell's row index in the grid
     * @param columnIndex A cell's column index in the grid
     * @return The "box" index for that cell
     */
    static int8_t getBoxIndex(const int8_t rowIndex, const int8_t columnIndex);

    /**
     * Creates all [row][column] index pairs for the cells of every box
     * @return A vector of vectors of index pairs. Outer vector references each box, inner vector contains all index pairs for that box
     */
    static std::vector<std::vector<std::pair<int8_t, int8_t>>> getBoxIndices();

};

