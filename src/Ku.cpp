#include "Ku.h"
#include <memory>
#include "sudoku/Sudoku.h"
#include "utilities/Utilities.h"

void Ku::run() {
    const ConstraintType constraints = ConstraintType::SUDOKU_BOX | ConstraintType::SUDOKU_COLUMN | ConstraintType::SUDOKU_ROW;
    std::unique_ptr<Sudoku> sudoku = std::make_unique<Sudoku>(50, SetterType::RANDOM, constraints);
}
