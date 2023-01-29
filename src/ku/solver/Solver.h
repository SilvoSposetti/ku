#pragma once

#include "../constraints/AbstractConstraint.h"
#include "Node.h"

class Solver {
public:
  /** Creates a new board from scratch with the given constraints using DLX
   * @param constraints A vector of constraints
   * @return A board constructed according to the input constraints
   */
  static std::vector<std::vector<Sudo>>
  createNewBoard(const std::vector<std::unique_ptr<AbstractConstraint>>& constraints);

  /** Computes whether a Sudoku is unique according to the provided solution, givenMask, and set of constraints
   * @param solution The board solution
   * @param givenMask The given mask
   * @param constraints The constraints which the solution satisfies
   * @return givenMask
   */
  static bool isUnique(const std::vector<std::vector<Sudo>>& solution,
                       const std::vector<std::vector<bool>>& givenMask,
                       const std::vector<std::unique_ptr<AbstractConstraint>>& constraints);

  /** Creates and returns the sparse matrix of integers which could be used for DLX
   * @param board A (partial) board
   * @param constraints The set of constraints
   * @param randomize Whether matrix creation should randomize the order in which the digits are considered
   * @return A sparse matrix representation of the one used by DLX
   */
  static std::vector<std::vector<int32_t>>
  getDlxMatrix(const std::vector<std::vector<Sudo>>& board,
               const std::vector<std::unique_ptr<AbstractConstraint>>& constraints,
               bool randomize = false);

  /** Preliminary check to see if a set of constraint won't produce a solvable DLX problem. I.e. when the columns that
   * they define are all secondary
   * @param constraints The set of constraints
   * @return Whether the set of constraint is solvable
   */
  static bool isSolvable(const std::vector<std::unique_ptr<AbstractConstraint>>& constraints);

private:
  /** Solves a board using DLX
   * @param board A (partial) board that needs to be solved
   * @param constraints The set of constraints that the board should satisfy
   * @param checkForUniqueness Whether the solution found should be unique
   * @param randomize Whether matrix creation should randomize the order in which the digits are considered
   * @return Whether a solution exits, and if it has been specified, whether it is unique
   */
  static bool dlx(std::vector<std::vector<Sudo>>& board,
                  const std::vector<std::unique_ptr<AbstractConstraint>>& constraints,
                  bool checkForUniqueness,
                  bool randomize = false);

  /** Given a sparse matrix representation of the DLX-Matrix, constructs a matrix of Nodes to be used by the DLX
   * algorithm
   * @param matrix A sparse matrix representation of the one used by DLX
   * @param constraints The set of constraints tha the board should satisfy
   * @return The root of the constructed matrix of Nodes
   */
  static std::shared_ptr<Node>
  createDancingLinksMatrix(const std::vector<std::vector<int32_t>>& matrix,
                           const std::vector<std::unique_ptr<AbstractConstraint>>& constraints);

  /** Performs the actual search of solutions of DLX
   * @param root The root to the matrix of Nodes used by DLX
   * @param checkForUniqueness Whether DLX should check for uniqueness
   * @return One or multiple vector of Nodes that describe the solution(s) found
   */
  static std::vector<std::vector<std::shared_ptr<Node>>> searchDlx(const std::shared_ptr<Node>& root,
                                                                   bool checkForUniqueness);

  /** Performs the recursive and backtracking search of DLX
   * @param root The root to the matrix of Nodes used by DLX
   * @param depth The current depth of the recursion tree
   * @param solutionsLeftToSearchFor How many solutions still need to be found
   * @param solutionHolder A container to store current Nodes that might be a solution
   * @param solutions A general container to store all previously found solutions
   */
  static void searchDlxRecursive(const std::shared_ptr<Node>& root,
                                 int32_t depth,
                                 int32_t& solutionsLeftToSearchFor,
                                 std::vector<std::shared_ptr<Node>>& solutionHolder,
                                 std::vector<std::vector<std::shared_ptr<Node>>>& solutions);

  /** Performs the "covering" of a columnHeader within the DLX-matrix of Nodes
   * Potentially modifies the links between the nodes of the column
   * @param columnHeader The header that needs to be covered
   */
  static void cover(std::shared_ptr<Node>& columnHeader);

  /** Performs the "uncovering" of a columnHeader within the DLX-matrix of Nodes.
   * Potentially modifies the links between the nodes of the column
   * @param columnHeader The header that needs to be uncovered
   */
  static void uncover(std::shared_ptr<Node>& columnHeader);

  /** Picks the smallest column reachable from the root. Smallest meaning the one with less Nodes under it
   */
  static std::shared_ptr<Node> chooseSmallestColumn(const std::shared_ptr<Node>& root);

  /** Checks if a given sparse matrix represents a valid Exact Cover problem to be fed to DLX
   */
  static bool isMatrixSolvable(const std::vector<std::vector<int32_t>>& matrix,
                               const std::vector<std::unique_ptr<AbstractConstraint>>& constraints);
};
