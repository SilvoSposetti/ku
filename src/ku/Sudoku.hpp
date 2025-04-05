#pragma once

#include "Board.hpp"
#include "Sudo.hpp"
#include "constraints/AbstractConstraint.hpp"
#include "constraints/ConstraintType.hpp"

#include <filesystem>
#include <optional>
#include <vector>

/** Main class that stores Sudokus
 */
class Sudoku {
public:
  /** Constructor without given digits
   * @param name The name of the Sudoku
   * @param constraintTypes Bitflag of the constraints that should be used to create the Sudoku
   * @param givenDigits The amount of digits that are given
   * @param seed The seed for the random number generator used to generate and solve the sudoku. If -1 then a random
   * seed will be used
   */
  Sudoku(const std::string& name,
         ConstraintType constraintTypes,
         int32_t givenDigits = Sudo::TOTAL_DIGITS,
         std::optional<int32_t> seed = std::nullopt);

  /** Constructor with some given digits
   * @param name The name of the Sudoku
   * @param clues A matrix of given digits where 0 is an empty cell of the grid
   * @param constraintTypes Bitflag of the constraints that should be used to create the Sudoku
   * @param seed The seed for the random number generator used to generate and solve the sudoku. If -1 then a random
   * seed will be used
   */
  Sudoku(const std::string& name,
         const std::vector<std::vector<int32_t>>& clues,
         ConstraintType constraintTypes,
         std::optional<int32_t> seed = std::nullopt);

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

  /** Retrieve the solution of the sudoku
   * \return The solution
   */
  std::vector<std::vector<Sudo::Digit>> getSolution();

  /** Compute and retrieve the field of the sudoku. This is only the given digits of the solution
   * \return The field
   */
  std::vector<std::vector<Sudo::Digit>> getField();

  /** Retrieve the given mask of the sudoku. An element is true if it is given
   * \return The given mask
   */
  std::vector<std::vector<bool>> getGivenMask();

  /** Generates and stores the sudoku board to an SVG file in the /out directory
   * @param location Where the file should be stored
   */
  void exportToSvg(const std::filesystem::path& location);

  /** Generates and stores the Exact Cover matrix of this Sudoku to an SVG file in the /out directory
   * @param location Where the file should be stored
   */
  void exportExactCoverMatrixToSvg(const std::filesystem::path& location);

  /** Logs sudoku's info to stdout
   */
  void printInfo();

  /** Logs the board to stdout
   */
  void printBoard();

  /** Constructs a vector of built constraints according to the given constraint types
   * @param constraintTypes Bitflag of the constraints that should be constructed
   * @return A vector of constraints
   */
  static std::vector<std::unique_ptr<AbstractConstraint>> getConstraintsList(const ConstraintType constraintTypes);

private:
  /** Helper to compute the amount of clues
   * @return The amount of clues
   */
  int32_t getGivenDigitsAmount() const;

  /** Helper used to transform a matrix of integers into a matrix of Sudo::Digits
   * @param clues The board of clues
   * @return The transformed matrix
   */
  static std::vector<std::vector<Sudo::Digit>> transformClues(const std::vector<std::vector<int32_t>>& clues);

private:
  /// The name
  const std::string name;
  /// The list of constraint that make up this Sudoku
  const std::vector<std::unique_ptr<AbstractConstraint>> constraints;
  /// The seed for the random numbers generator
  std::optional<int32_t> seed;
  /// The board
  const std::unique_ptr<Board> board;
};
