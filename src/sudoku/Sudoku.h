#pragma once
#include "Board.h"
#include "constraints/AbstractConstraint.h"
#include <memory>

class Sudoku {
public:
    Sudoku(const int8_t totalDigits, const SetterType setterType, const ConstraintType constraintTypes);

private:
    int8_t digitsAmount;
    std::unique_ptr<Board> board;
    std::vector<std::unique_ptr<AbstractConstraint>> constraints;
};
