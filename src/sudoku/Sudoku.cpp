#include "Sudoku.h"
#include "Setter.h"
#include "constraints/ConstraintFactory.h"

Sudoku::Sudoku(const u_int8_t totalDigits, const SetterType setterType, const ConstraintType constraintTypes) {
    digitsAmount = totalDigits;
    digitsAmount = digitsAmount <= 0 ? defaultDigits : totalDigits;
    digitsAmount = digitsAmount >= 81 ? defaultDigits : totalDigits;

    for (uint8_t bitToCheck = 0; bitToCheck < 64; bitToCheck++) {
        const uint64_t valueToCheck = static_cast<uint64_t>(1) << bitToCheck;
        if (static_cast<uint64_t>(constraintTypes) & valueToCheck) {
            const ConstraintType singleConstraint = static_cast<ConstraintType>(valueToCheck);
            constraints.emplace_back(ConstraintFactory::makeConstraint(singleConstraint));
        }
    }
    for (const auto& constraint: constraints) {
        std::cout << constraint->getName() << ":\t" << constraint->getDescription() << std::endl;
    }

    board = Setter::generate(digitsAmount, setterType, constraints);
    board->printAll();
}
