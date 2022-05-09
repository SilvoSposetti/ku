#pragma once

#include "Board.h"
#include "constraints/AbstractConstraint.h"

class Sudoku {
public:
  Sudoku(const std::string& name, int32_t totalDigits, ConstraintType constraintTypes, SymmetryType symmetryType);

  void exportSudokuToSvg();
  void exportDlxMatrixToSvg();

private:
  static std::vector<std::unique_ptr<AbstractConstraint>> getConstraintsList(const ConstraintType constraintTypes);

private:
  std::string name;
  int32_t digitsAmount;
  std::unique_ptr<Board> board;
  std::vector<std::unique_ptr<AbstractConstraint>> constraints;
};
