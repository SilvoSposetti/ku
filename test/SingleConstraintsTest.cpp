
#include "Sudoku.h"
#include "constraints/ConstraintFactory.h"
#include "constraints/ConstraintType.h"

#include <doctest.h>

TEST_CASE("SingleConstraintsTest") {
 
  SUBCASE("No base constraints, no givens") {
    const std::filesystem::path location = std::filesystem::path(OUT_DIR) / "tests" / "SingleConstraintOnly";

    for (const auto& constraintType : Constraint::ALL_CONSTRAINTS_SET) {
      const std::string constraintName = Constraint::getConstraintNameString(constraintType);
      Sudoku sudoku(constraintName + "-Only", constraintType);
      if (sudoku.isSolvable()) { // Do not verify sudokus' whose constraints do not define primary columns
        CHECK(sudoku.verify());
      }
      sudoku.exportToSvg(location);
      sudoku.exportDlxMatrixToSvg(location);
    }
  }

  SUBCASE("Sudoku base constraints, no givens") {
    const std::filesystem::path location = std::filesystem::path(OUT_DIR) / "tests" / "SingleConstraintWithSudokuBase";

    const ConstraintType sudokuBaseConstraints = Constraint::getSudokuConstraints();

    for (const auto& constraintType : Constraint::ALL_CONSTRAINTS_SET) {
      // Add only constraints that are not the base ones
      if (!static_cast<bool>(constraintType & sudokuBaseConstraints)) {
        const std::string constraintName = Constraint::getConstraintNameString(constraintType);
        Sudoku sudoku("SudokuBase-and-" + constraintName, constraintType | sudokuBaseConstraints);
        CHECK(sudoku.verify());
        sudoku.exportToSvg(location);
        sudoku.exportDlxMatrixToSvg(location);
      }
    }
  }
}
