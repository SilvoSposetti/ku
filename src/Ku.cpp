#include "Ku.h"
#include <memory>
#include "sudoku/Sudoku.h"
#include "utilities/Utilities.h"

void Ku::run() {
    const ConstraintType constraints = ConstraintType::SUDOKU_BOX | ConstraintType::SUDOKU_COLUMN | ConstraintType::SUDOKU_ROW;
    std::unique_ptr<Sudoku> sudoku1 = std::make_unique<Sudoku>(50, SetterType::RANDOM, constraints);
    std::unique_ptr<Sudoku> sudoku2 = std::make_unique<Sudoku>(30, SetterType::RANDOM, constraints);
    std::unique_ptr<Sudoku> sudoku3 = std::make_unique<Sudoku>(10, SetterType::RANDOM, constraints);
}
