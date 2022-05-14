#include "Ku.h"

#include "sudoku/Sudoku.h"
#include "utilities/Utilities.h"

void Ku::run() {
  ConstraintType constraints = ConstraintType::NONE;
  constraints = constraints | ConstraintType::SUDOKU_ROW;
  constraints = constraints | ConstraintType::SUDOKU_COLUMN;
  constraints = constraints | ConstraintType::SUDOKU_BOX;
  // constraints = constraints | ConstraintType::POSITIVE_DIAGONAL;
  // constraints = constraints | ConstraintType::POSITIVE_DIAGONAL_EVEN;
  constraints = constraints | ConstraintType::POSITIVE_DIAGONAL_ODD;
  // constraints = constraints | ConstraintType::NEGATIVE_DIAGONAL;
  // constraints = constraints | ConstraintType::KINGS_MOVE;

  for (int32_t i = 0; i < 1; ++i) {
    const SymmetryType symmetryType = static_cast<SymmetryType>(i % static_cast<int32_t>(SymmetryType::AMOUNT));
    std::unique_ptr<Sudoku> sudoku =
        std::make_unique<Sudoku>("Sudoku" + std::to_string(i + 1), 80, constraints, symmetryType);
  }
}
