#pragma once

#include "../constraints/AbstractConstraint.h"
#include "../randomGenerator/RandomGenerator.h"
#include "SparseCoordinateMatrix.h"

#include <memory>

class Solver {
public:
  /** Creates a new board from scratch with the given constraints using Algorithm X
   * @param constraints A vector of constraints
   * @param randomGenerator The random number generator instance used to draw random numbers
   * @return A board constructed according to the input constraints
   */
  static std::vector<std::vector<Sudo::Digit>>
  createNewBoard(const std::vector<std::unique_ptr<AbstractConstraint>>& constraints,
                 std::shared_ptr<RandomGenerator> randomGenerator);

  /** Computes whether a Sudoku is unique according to the provided solution, givenMask, and set of constraints
   * @param solution The board solution
   * @param givenMask The given mask
   * @param constraints The constraints which the solution satisfies
   * @return givenMask
   */
  static bool isUnique(const std::vector<std::vector<Sudo::Digit>>& solution,
                       const std::vector<std::vector<bool>>& givenMask,
                       const std::vector<std::unique_ptr<AbstractConstraint>>& constraints);

  static SparseCoordinateMatrix
  getExactCoverMatrix(const std::vector<std::unique_ptr<AbstractConstraint>>& constraints);

  /** Preliminary check to see if a set of constraint won't produce a solvable Algorithm X problem. I.e. when the
   * columns that they define are all secondary
   * @param constraints The set of constraints
   * @return Whether the set of constraint is solvable
   */
  static bool isSolvable(const std::vector<std::unique_ptr<AbstractConstraint>>& constraints);

private:
  /** Solves a board using Algorithm X
   * @param board A (partial) board that needs to be solved
   * @param constraints The set of constraints that the board should satisfy
   * @param checkForUniqueness Whether the solution found should be unique
   * @param randomGenerator The random number generator instance used to draw random numbers
   * @return Whether a solution exits, and if it has been specified, whether it is unique
   */
  static bool solve(std::vector<std::vector<Sudo::Digit>>& board,
                    const std::vector<std::unique_ptr<AbstractConstraint>>& constraints,
                    bool checkForUniqueness,
                    std::shared_ptr<RandomGenerator> randomGenerator = nullptr);

  /** Creates and returns the sparse matrix of integers which could be used for Algorithm X
   * @param board A (partial) board
   * @param constraints The set of constraints
   * @param randomGenerator The random number generator instance used to draw random numbers
   * @return A sparse matrix representation of the one used by Algorithm X
   */
  static SparseCoordinateMatrix
  reduceSudokuProblemToExactCoverProblem(const std::vector<std::vector<Sudo::Digit>>& board,
                                         const std::vector<std::unique_ptr<AbstractConstraint>>& constraints,
                                         std::shared_ptr<RandomGenerator> randomGenerator = nullptr);

  /** Creates a Sudoku board from an Exact Cover matrix and one of its (possibly many) solutions
   * @param matrix The Exact Cover problem's sparse matrix
   * @param rowIndicesSet A set of matrix rows that solve the Exact Cover problem in the matrix
   * @return A Sudoku board constructed with the solution provided
   */
  static void reduceExactCoverSolutionToSudokuSolution(std::vector<std::vector<Sudo::Digit>>& board,
                                                       const SparseCoordinateMatrix& matrix,
                                                       const std::unordered_set<int32_t>& solutionRows);
};
