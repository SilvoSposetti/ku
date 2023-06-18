#include "Solver.h"

#include "../Sudo.h"
#include "../Validator.h"
#include "../randomGenerator/RandomGenerator.h"
#include "AlgorithmX.h"

std::vector<std::vector<Sudo::Digit>>
Solver::createNewBoard(const std::vector<std::unique_ptr<AbstractConstraint>>& constraints,
                       std::shared_ptr<RandomGenerator> randomGenerator) {

  std::vector<std::vector<Sudo::Digit>> newField = Sudo::emptyField();
  std::vector<std::vector<bool>> givenMask = Sudo::emptyGivenMask();

  const bool created = Solver::solve(newField, constraints, false, randomGenerator);

  if (!created) {
    std::string constraintsNames;
    for (const auto& constraint : constraints) {
      if (constraint->getType() != ConstraintType::SUDOKU_CELL) {

        constraintsNames += constraint->getName() + ", ";
      }
    }
    if (!constraintsNames.empty()) {
      constraintsNames = constraintsNames.substr(0, constraintsNames.size() - 2);
    }

    std::cout << "ERROR: Was not able to generate a new board with the given constraints: (" << constraintsNames << ")"
              << std::endl;
  } else if (!Validator::checkSolution(newField, constraints)) {
    std::cout << "ERROR: Solution created does not satisfy all constraints" << std::endl;
  }
  return newField;
}

bool Solver::isUnique(const std::vector<std::vector<Sudo::Digit>>& solution,
                      const std::vector<std::vector<bool>>& givenMask,
                      const std::vector<std::unique_ptr<AbstractConstraint>>& constraints) {

  // Create board to use for solving
  std::vector<std::vector<Sudo::Digit>> board = solution;
  for (const auto& i : Sudo::INDICES) {
    for (const auto& j : Sudo::INDICES) {
      if (!givenMask[i][j]) {
        board[i][j] = Sudo::Digit::NONE;
      }
    }
  }
  return Solver::solve(board, constraints, true);
}

SparseCoordinateMatrix
Solver::getExactCoverMatrix(const std::vector<std::unique_ptr<AbstractConstraint>>& constraints) {

  std::vector<std::vector<Sudo::Digit>> board = Sudo::emptyField();
  return reduceSudokuProblemToExactCoverProblem(board, constraints);
}

bool Solver::isSolvable(const std::vector<std::unique_ptr<AbstractConstraint>>& constraints) {
  bool isSecondaryColumnsOnly = true;
  for (const auto& constraint : constraints) {
    isSecondaryColumnsOnly &= constraint->isSecondaryColumnsOnly();
  }
  return isSecondaryColumnsOnly;
}

bool Solver::solve(std::vector<std::vector<Sudo::Digit>>& board,
                   const std::vector<std::unique_ptr<AbstractConstraint>>& constraints,
                   bool checkForUniqueness,
                   std::shared_ptr<RandomGenerator> randomGenerator) {
  // Reduce problem: Sudoku -> Exact Cover
  SparseCoordinateMatrix matrix = reduceSudokuProblemToExactCoverProblem(board, constraints, randomGenerator);

  // Check that the matrix is valid
  if (!matrix.isSolvableByAlgorithmX()) {
    return false;
  }

  // No need to reduce the solution back to a valid board when simply checking for uniqueness
  if (checkForUniqueness) {
    return AlgorithmX::hasUniqueSolution(matrix);
  }
  // AlgorithmX::printDataStructure(matrix);

  // Find a possible solution
  std::unordered_set<int32_t> solution = AlgorithmX::findOneSolution(matrix);

  // Use first solution out of all those that are found
  // This solution should have 81 elements, one for each cell piked
  if (solution.size() == Sudo::TOTAL_DIGITS) {
    // Reduce solution: Exact Cover -> Sudoku
    reduceExactCoverSolutionToSudokuSolution(board, matrix, solution);

    return true;
  }
  return false;
}

SparseCoordinateMatrix
Solver::reduceSudokuProblemToExactCoverProblem(const std::vector<std::vector<Sudo::Digit>>& board,
                                               const std::vector<std::unique_ptr<AbstractConstraint>>& constraints,
                                               std::shared_ptr<RandomGenerator> randomGenerator) {
  // To initialize the matrix with the correct size: count how many digits are given
  int32_t givenAmount = 0;
  for (const auto& row : board) {
    for (const auto& digit : row) {
      if (digit != Sudo::Digit::NONE) {
        ++givenAmount;
      }
    }
  }
  // 729 rows => (81 cells, 9 possible digits for each cell)
  constexpr int32_t maximumRows = 9 * 9 * 9;
  // Each given reduces the amount of rows by (Sudo::MAX_DIGIT - 1)
  const int32_t totalRows = maximumRows - (Sudo::MAX_DIGIT - 1) * givenAmount;

  int32_t totalColumns = 0;
  for (const auto& constraint : constraints) {
    totalColumns += constraint->getDlxConstraintColumnsAmount();
  }

  // Initialize matrix with correct size
  SparseCoordinateMatrix matrix(totalRows, totalColumns);
  // Randomize the sequence of digits that is passed when constructing the matrix or not
  const std::vector<Sudo::Digit> digitsSequence =
      randomGenerator ? randomGenerator->randomShuffle(Sudo::SUDO_DIGITS) : Sudo::SUDO_DIGITS;

  int32_t matrixRowCounter = 0;
  int32_t matrixColumnCounter = 0;
  for (const auto& boardI : Sudo::INDICES) { // Go through all sudoku rows
    for (const auto& boardJ : Sudo::INDICES) { // Go through all sudoku column
      const Sudo::Digit actualDigit = board[boardI][boardJ];
      for (const auto& possibleDigit : digitsSequence) { // Go through all possible digits for this cell
        // Avoid rows where the cell and its possible digits don't create a "1" in the matrix, since the digit
        // is already given
        if (actualDigit == Sudo::Digit::NONE || actualDigit == possibleDigit) {
          for (const auto& constraint : constraints) {
            for (int32_t columnId = 0; columnId < constraint->getDlxConstraintColumnsAmount(); ++columnId) {
              if (!constraint->isColumnPrimary(columnId)) {
                matrix.setColumnSecondary(matrixColumnCounter);
              }
              if (constraint->getDlxConstraint(possibleDigit, boardI, boardJ, columnId)) {
                // Store matrix cell ID
                matrix.setCell(matrixRowCounter,
                               matrixColumnCounter,
                               boardI * Sudo::TOTAL_DIGITS + boardJ * Sudo::MAX_DIGIT +
                                   (static_cast<int32_t>(possibleDigit) - 1));
              }
              matrixColumnCounter = (matrixColumnCounter + 1) % totalColumns;
            }
          }
          matrixRowCounter++;
        }
      }
    }
  }

  return matrix;
}

void Solver::reduceExactCoverSolutionToSudokuSolution(std::vector<std::vector<Sudo::Digit>>& board,
                                                      const SparseCoordinateMatrix& matrix,
                                                      const std::unordered_set<int32_t>& solutionRows) {
  const int32_t columnsAmount = matrix.getColumnsAmount();

  for (const auto& rowIndex : solutionRows) {
    int32_t dataRetrieved = -1;
    int32_t columnIndex = 0;
    do {
      dataRetrieved = matrix.getCell(rowIndex, columnIndex);
      columnIndex++;
    } while (dataRetrieved < 0 && columnIndex < columnsAmount);
    // This uses the same method used to identify the cells of the SUDOKU_CELL constraint,
    // but the process here is reversed
    const int32_t boardRow = dataRetrieved / Sudo::TOTAL_DIGITS;
    const int32_t boardColumn = (dataRetrieved / Sudo::MAX_DIGIT) % Sudo::MAX_DIGIT;
    const Sudo::Digit actualDigit = static_cast<Sudo::Digit>((dataRetrieved % Sudo::MAX_DIGIT) + 1);
    board[boardRow][boardColumn] = actualDigit;
  }
}
