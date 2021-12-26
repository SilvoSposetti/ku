#pragma once
#include "Board.h"
#include "constraints/AbstractConstraint.h"
#include <memory>

class Sudoku {
public:
    Sudoku(const u_int8_t totalDigits, const SetterType setterType, const ConstraintType constraintTypes);

private:
    static constexpr uint8_t defaultDigits = 60;

    uint8_t digitsAmount;
    std::unique_ptr<Board> board;
    std::vector<std::unique_ptr<AbstractConstraint>> constraints;
};
