#include "ConstraintFactory.h"
#include "SudokuCell.h"
#include "SudokuRow.h"
#include "SudokuColumn.h"
#include "SudokuBox.h"

std::unique_ptr<AbstractConstraint> ConstraintFactory::makeConstraint(ConstraintType constraintType) {
    switch (constraintType) {
        case ConstraintType::SUDOKU_CELL:
            return std::make_unique<SudokuCell>();
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

    return std::make_unique<SudokuCell>();
}
