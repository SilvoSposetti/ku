#include "Sudoku.h"
#include "Setter.h"
#include "constraints/ConstraintFactory.h"
#include <cstdint>

Sudoku::Sudoku(const std::string& name,
               int32_t totalDigits,
               ConstraintType constraintTypes,
               SymmetryType symmetryType) {
    
    // Retrieve constraint list
    constraints = getConstraintsList(constraintTypes);

    // Minimum for unique solution: 17
    if (constraintTypes == (ConstraintType::SUDOKU_COLUMN | ConstraintType::SUDOKU_ROW | ConstraintType::SUDOKU_BOX)){
        digitsAmount = clamp(totalDigits, 17, 81);
    }
    else{
        digitsAmount  = clamp(totalDigits, 0, 81);
    }


 

    board = Setter::generate(digitsAmount, symmetryType, constraints);

    std::cout << name << " [" << digitsAmount << "|-" << std::to_string(81 - digitsAmount) << "]    MaskType: "
              << std::to_string(static_cast<int>(symmetryType)) << std::endl;
    board->print();
    // board->printGivenPattern();
    std::cout << std::endl;
}


std::vector<std::unique_ptr<AbstractConstraint>> Sudoku::getConstraintsList(const ConstraintType constraintTypes){
    std::vector<std::unique_ptr<AbstractConstraint>> constraintList;
    
    // SUDOKU_CELL constraint is always present
    constraintList.emplace_back(ConstraintFactory::makeConstraint(ConstraintType::SUDOKU_CELL));

    for (int8_t bitToCheck = 0; bitToCheck < 64; bitToCheck++) {
        const uint64_t valueToCheck = static_cast<uint64_t>(1) << bitToCheck;
        if (static_cast<uint64_t>(constraintTypes) & valueToCheck) {
            const ConstraintType singleConstraint = static_cast<ConstraintType>(valueToCheck);
            if (singleConstraint != ConstraintType::SUDOKU_CELL){
                constraintList.emplace_back(ConstraintFactory::makeConstraint(singleConstraint));
            }
        }
    }

    return constraintList;
}
