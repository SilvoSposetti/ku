#pragma once

#include "Board.h"
#include "constraints/AbstractConstraint.h"

/** Main class that stores Sudokus
 */
class Sudoku {
public:
  /** Constructor
   * @param name The name of the Sudoku
   * @param constraintTypes Bitflag of the constraints that should be used to create the Sudoku
   * @param symmetryType Which symmetry type should be used to remove given digits
   */
  Sudoku(const std::string& name, int32_t totalDigits, ConstraintType constraintTypes, SymmetryType symmetryType);

  /** Generates and stores the sudoku board to an SVG file in the /out directory
   */
  void exportSudokuToSvg();

  /** Generates and stores the DLX matrix of this Sudoku to an SVG file in the /out directory
   */
  void exportDlxMatrixToSvg();

private:
  /** Constructs a vector of built constraints according to the given constraint types
   * @param constraintTypes Bitflag of the constraints that should be constructed
   * @return A vector of constraints
   */
  static std::vector<std::unique_ptr<AbstractConstraint>> getConstraintsList(const ConstraintType constraintTypes);

private:
  /// The name
  std::string name;
  /// How many givens the sudoku has
  int32_t digitsAmount;
  /// The board
  std::unique_ptr<Board> board;
  /// The list of constraint that make up this Sudoku
  std::vector<std::unique_ptr<AbstractConstraint>> constraints;
};
