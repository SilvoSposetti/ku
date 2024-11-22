#pragma once

#include "../Sudo.h"
#include "../constraints/AbstractConstraint.h"
#include "OptionData.h"

#include <memory>
#include <optional>
#include <unordered_set>
#include <vector>

class Solver {
public:
  /** Creates a new board from scratch with the given constraints using Algorithm C
   * @param constraints A vector of constraints
   * @param seed The seed for the random number generator used to run Algorithm C
   * @return A board constructed according to the input constraints
   */
  static std::vector<std::vector<Sudo::Digit>>
  createNewBoard(const std::vector<std::unique_ptr<AbstractConstraint>>& constraints, std::optional<int32_t> seed);

  /** Fills an existing board with the given digits and constraints using Algorithm C
   * @param clues A set of given digits
   * @param constraints A vector of constraints
   * @param seed The seed for the random number generator used to run Algorithm C
   * @return A board constructed according to the input constraints
   */
  static std::vector<std::vector<Sudo::Digit>>
  fillExistingBoard(const std::vector<std::vector<Sudo::Digit>>& clues,
                    const std::vector<std::unique_ptr<AbstractConstraint>>& constraints,
                    std::optional<int32_t> seed);

  /** Computes whether a Sudoku is unique according to the provided solution, givenMask, and set of constraints
   * @param field The field
   * @param constraints The constraints which the solution satisfies
   * @return givenMask
   */
  static bool isUnique(const std::vector<std::vector<Sudo::Digit>>& field,
                       const std::vector<std::unique_ptr<AbstractConstraint>>& constraints);

  /** Preliminary check to see if a set of constraints won't produce a solvable Algorithm C problem. E.g. when the
   * columns that they define are all secondary
   * @param constraints The set of constraints
   * @return Whether the set of constraint is solvable
   */
  static bool isSolvable(const std::vector<std::unique_ptr<AbstractConstraint>>& constraints);

private:
  /** Solves a board using Algorithm C
   * @param board A (partial) board that needs to be solved
   * @param constraints The set of constraints that the board should satisfy
   * @param checkForUniqueness Whether the solution found should be unique
   * @param seed The seed for the random number generator used to run Algorithm C
   * @return Whether a solution exits, and if it has been specified, whether it is unique
   */
  static bool solve(std::vector<std::vector<Sudo::Digit>>& board,
                    const std::vector<std::unique_ptr<AbstractConstraint>>& constraints,
                    bool checkForUniqueness,
                    std::optional<int32_t> seed);

  /** Creates a Sudoku board from an Exact Cover matrix and one of its (possibly many) solutions
   * @param board The Sudoku problem's board to be solved
   * @param optionsData The data about the options
   * @param solutionOptions A of option indices that solve the Exact Cover problem
   */
  static void reduceExactCoverSolutionToSudokuSolution(std::vector<std::vector<Sudo::Digit>>& board,
                                                       const std::vector<OptionData>& optionsData,
                                                       const std::unordered_set<int32_t>& solutionOptions);
};
