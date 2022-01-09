#include "Sudoku.h"
#include "Setter.h"
#include "constraints/ConstraintFactory.h"

Sudoku::Sudoku(const std::string& name,
               const int8_t totalDigits,
               const SymmetryType setterType,
               const ConstraintType constraintTypes) {
    digitsAmount = clamp(totalDigits, 17, 81);

    for (int8_t bitToCheck = 0; bitToCheck < 64; bitToCheck++) {
        const uint64_t valueToCheck = static_cast<uint64_t>(1) << bitToCheck;
        if (static_cast<uint64_t>(constraintTypes) & valueToCheck) {
            const ConstraintType singleConstraint = static_cast<ConstraintType>(valueToCheck);
            constraints.emplace_back(ConstraintFactory::makeConstraint(singleConstraint));
        }
    }

    board = Setter::generate(digitsAmount, setterType, constraints);
    std::cout << name << " [-" << std::to_string(81 - totalDigits) << "]                   MaskType: "
              << std::to_string(static_cast<int>(setterType)) << std::endl;
    board->print();
//    board->printGivenPattern();
    std::cout << std::endl;
}
