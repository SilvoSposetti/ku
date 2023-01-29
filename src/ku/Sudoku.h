#pragma once

#include "Board.h"
#include "constraints/AbstractConstraint.h"
#include "randomGenerator/RandomGenerator.h"
#include "utilities/Utilities.h"

/** Main class that stores Sudokus
 */
class Sudoku {
public:
  /** Constructor
   * @param name The name of the Sudoku
   * @param constraintTypes Bitflag of the constraints that should be used to create the Sudoku
   * @param givenDigits The amount of digits that are given
   * @param symmetryType Which symmetry type should be used to remove given digits. Default: SymmetryType::RANDOM.
   */
  Sudoku(const std::string& name,
         ConstraintType constraintTypes,
         int32_t givenDigits = TOTAL_DIGITS,
         SymmetryType symmetryType = SymmetryType::RANDOM);

  /** Check if the sudoku is solvable. This is not the case where the constraints define secondary columns only
   * @return Whether the sudoku is solvable
   */
  bool isSolvable();

  /** Verify the given sudoku, meaning both checking for:
   * 1. The solution satisfies the constraints.
   * 2. Solving the sudoku produces the solution uniquely.
   * @return Whether the sudoku si valid
   */
  bool verify();

  /** Generates and stores the sudoku board to an SVG file in the /out directory
   * @param location Where the file should be stored
   */
  void exportToSvg(const std::filesystem::path& location);

  /** Generates and stores the DLX matrix of this Sudoku to an SVG file in the /out directory
   * @param location Where the file should be stored
   */
  void exportDlxMatrixToSvg(const std::filesystem::path& location);

  /** Logs sudoku's info to stdout
   */
  void printInfo();

  /** Logs the board to stdout
   */
  void printBoard();

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
  int32_t givenDigitsAmount;
  /// The board
  std::unique_ptr<Board> board;
  /// The list of constraint that make up this Sudoku
  std::vector<std::unique_ptr<AbstractConstraint>> constraints;
  /// The random generator
  RandomGenerator randomGenerator;
};
