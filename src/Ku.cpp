#include "Ku.h"

#include "sudoku/Sudoku.h"
#include "utilities/Utilities.h"

void Ku::run() {
  const ConstraintType constraints =
      ConstraintType::SUDOKU_BOX | ConstraintType::SUDOKU_COLUMN | ConstraintType::SUDOKU_ROW;

  for (int i = 0; i < 1; i++) {
    const SymmetryType symmetryType = static_cast<SymmetryType>(i % static_cast<int32_t>(SymmetryType::AMOUNT));
    std::unique_ptr<Sudoku> sudoku =
        std::make_unique<Sudoku>("Sudoku " + std::to_string(i + 1), 30, constraints, symmetryType);
    sudoku->exportToSvg();
  }
}
