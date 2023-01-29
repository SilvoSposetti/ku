
#include "Sudoku.h"
#include "constraints/ConstraintFactory.h"
#include "constraints/ConstraintType.h"

#include <doctest.h>

TEST_CASE("Single constraint only, no missing digits") {
  const std::filesystem::path location = std::filesystem::path(OUT_DIR) / "tests" / "SingleConstraintOnly";

  for (const auto& constraintType : Constraint::ALL_CONSTRAINTS) {
    const std::string constraintName = Constraint::getConstraintNameString(constraintType);
    Sudoku sudoku(constraintName + "-Only", constraintType);
    if (sudoku.isSolvable()) { // Do not verify sudokus' whose constraints do not define primary columns
      CHECK(sudoku.verify());
    }
    sudoku.exportToSvg(location);
    sudoku.exportDlxMatrixToSvg(location);
  }
}

// TEST_CASE("Single constraint with Sudoku base rules, no missing digits") {
//   const std::filesystem::path location = std::filesystem::path(OUT_DIR) / "tests" / "SingleConstraintWithSudokuBase";
//   for (const auto& constraintType : Constraint::ALL_CONSTRAINTS) {
//     const std::string constraintName = Constraint::getConstraintNameString(constraintType);
//     Sudoku sudoku(constraintName, constraintType);
//     if (sudoku.isSolvable()) { // Do not verify sudokus' whose constraints do not define primary columns
//       CHECK(sudoku.verify());
//     }
//     sudoku.exportToSvg(location);
//     sudoku.exportDlxMatrixToSvg(location);
//   }
// }
