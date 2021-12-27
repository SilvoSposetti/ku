#include "Sudoku.h"
#include "Setter.h"
#include "constraints/ConstraintFactory.h"

Sudoku::Sudoku(const int8_t totalDigits, const SetterType setterType, const ConstraintType constraintTypes) {
    digitsAmount = clamp(totalDigits, 0, 81);

    for (int8_t bitToCheck = 0; bitToCheck < 64; bitToCheck++) {
        const uint64_t valueToCheck = static_cast<uint64_t>(1) << bitToCheck;
        if (static_cast<uint64_t>(constraintTypes) & valueToCheck) {
            const ConstraintType singleConstraint = static_cast<ConstraintType>(valueToCheck);
            constraints.emplace_back(ConstraintFactory::makeConstraint(singleConstraint));
        }
    }

    board = Setter::generate(digitsAmount, setterType, constraints);
//    board->print()
    std::cout << std::endl;
    board->printGivenPattern();
}
