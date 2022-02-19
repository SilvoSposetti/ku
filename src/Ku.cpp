#include "Ku.h"
#include <memory>
#include "sudoku/Sudoku.h"
#include "utilities/Utilities.h"

void Ku::run() {
    const ConstraintType constraints =
            ConstraintType::SUDOKU_CELL |
            ConstraintType::SUDOKU_BOX |
            ConstraintType::SUDOKU_COLUMN |
            ConstraintType::SUDOKU_ROW;
        
    for (int i = 0; i < 1; i++) {
        std::unique_ptr<Sudoku> sudoku1 = std::make_unique<Sudoku>(std::to_string(i + 1), 81, static_cast<SymmetryType>(i), constraints);
    }

}
