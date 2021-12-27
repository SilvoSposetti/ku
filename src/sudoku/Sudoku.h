#pragma once
#include "Board.h"
#include "constraints/AbstractConstraint.h"
#include <memory>

class Sudoku {
public:
    Sudoku(const std::string& name, const int8_t totalDigits, const SymmetryType setterType, const ConstraintType constraintTypes);

private:
    int8_t digitsAmount;
    std::unique_ptr<Board> board;
    std::vector<std::unique_ptr<AbstractConstraint>> constraints;
};
