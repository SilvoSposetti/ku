#include "ConstraintFactory.h"
#include "SudokuBox.h"
#include "SudokuRow.h"
#include "SudokuColumn.h"

std::unique_ptr<AbstractConstraint> ConstraintFactory::makeConstraint(ConstraintType constraintType) {
    switch (constraintType) {
        case ConstraintType::SUDOKU_BOX:
            return std::make_unique<SudokuBox>();
        case ConstraintType::SUDOKU_ROW:
            return std::make_unique<SudokuRow>();
        case ConstraintType::SUDOKU_COLUMN:
            return std::make_unique<SudokuColumn>();
        case ConstraintType::MAIN_DIAGONAL:
            break;
        case ConstraintType::ANTI_DIAGONAL:
            break;
        case ConstraintType::KINGS_MOVE:
            break;
    }
}
