#include "Solver.h"

#include "../Sudo.h"
#include "../Validator.h"
#include "../randomGenerator/RandomGenerator.h"
#include "../utilities/IdPacking.h"
#include "AlgorithmX.h"

#include <map>

std::vector<std::vector<Sudo::Digit>>
Solver::createNewBoard(const std::vector<std::unique_ptr<AbstractConstraint>>& constraints,
                       std::optional<int32_t> seed) {

  std::vector<std::vector<Sudo::Digit>> newField = Sudo::emptyField();
  std::vector<std::vector<bool>> givenMask = Sudo::emptyGivenMask();

  const bool created = Solver::solve(newField, constraints, false, seed);

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

std::vector<std::vector<Sudo::Digit>>
Solver::fillExistingBoard(const std::vector<std::vector<Sudo::Digit>>& clues,
                          const std::vector<std::unique_ptr<AbstractConstraint>>& constraints,
                          std::optional<int32_t> seed) {
  auto field = clues;
  const bool solved = Solver::solve(field, constraints, false, seed);
  if (solved) {
    return field;
  }
  return Sudo::emptyField();
}

bool Solver::isUnique(const std::vector<std::vector<Sudo::Digit>>& field,
                      const std::vector<std::unique_ptr<AbstractConstraint>>& constraints) {

  std::vector<std::vector<Sudo::Digit>> temporaryField = field;
  return Solver::solve(temporaryField, constraints, true, std::nullopt);
}

SparseCoordinateMatrix
Solver::getExactCoverMatrix(const std::vector<std::vector<Sudo::Digit>>& board,
                            const std::vector<std::unique_ptr<AbstractConstraint>>& constraints) {
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
                   std::optional<int32_t> seed) {
  // Reduce problem: Sudoku -> Exact Cover
  SparseCoordinateMatrix matrix = reduceSudokuProblemToExactCoverProblem(board, constraints);

  // Preprocess the matrix to get it ready for Algorithm X
  // matrix.preprocess(randomGenerator);

  // Check that the matrix is valid
  if (!matrix.isSolvableByAlgorithmX()) {
    return false;
  }

  // No need to reduce the solution back to a valid board when simply checking for uniqueness
  if (checkForUniqueness) {
    return AlgorithmX::hasUniqueSolution(matrix, seed);
  }
  // AlgorithmX::printDataStructure(matrix);

  // Find a possible solution
  std::unordered_set<int32_t> solution = AlgorithmX::findOneSolution(matrix, seed);

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
                                               const std::vector<std::unique_ptr<AbstractConstraint>>& constraints) {
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
  constexpr int32_t maximumRows = Sudo::MAX_DIGIT * Sudo::MAX_DIGIT * Sudo::MAX_DIGIT;
  // Each given reduces the amount of rows by (Sudo::MAX_DIGIT - 1)
  const int32_t totalRows = maximumRows - (Sudo::MAX_DIGIT - 1) * givenAmount;

  int32_t totalColumns = 0;
  for (const auto& constraint : constraints) {
    totalColumns += constraint->getDlxConstraintColumnsAmount();
  }

  // Set up constraints cache
  std::vector<std::vector<std::vector<int32_t>>> constraintsCache{constraints.size()};
  int32_t constraintCounter = 0;
  for (const auto& constraint : constraints) {
    constraintsCache[constraintCounter] = constraint->getDlxConstraints();
    constraintCounter++;
  }

  // Initialize matrix with correct size
  std::vector<std::vector<bool>> matrix(totalRows, std::vector<bool>(totalColumns, false));
  std::vector<int32_t> rowsData(totalRows, -1);
  std::unordered_set<int32_t> secondaryColumns;

  // Randomize the sequence of digits that is passed when constructing the matrix or not
  const std::vector<Sudo::Digit> digitsSequence = Sudo::SUDO_DIGITS;

  int32_t matrixRowCounter = 0;
  int32_t setMatrixRowData = -1;

  for (int32_t optionId = 0; optionId < maximumRows; optionId++) {
    const auto [boardI, boardJ, unpackedDigit] =
        IdPacking::unpackId(optionId, Sudo::MAX_DIGIT, Sudo::MAX_DIGIT, Sudo::MAX_DIGIT);
    const Sudo::Digit possibleDigit = static_cast<Sudo::Digit>(unpackedDigit + 1);
    const Sudo::Digit actualDigit = board[boardI][boardJ];
    if (actualDigit == Sudo::Digit::NONE || actualDigit == possibleDigit) {
      int32_t matrixColumnBase = 0;
      for (int32_t constraintIndex = 0; constraintIndex < constraintsCache.size(); constraintIndex++) {
        const std::vector<int>& currentCache = constraintsCache[constraintIndex][optionId];
        for (const auto& itemId : currentCache) {
          const int matrixColumnId = matrixColumnBase + itemId;
          if (!constraints[constraintIndex]->isColumnPrimary(itemId)) {
            secondaryColumns.insert(matrixColumnId);
          }
          // Set cell
          matrix[matrixRowCounter][matrixColumnId] = true;
          // Store row data in the matrix, this is used to reduce Exact Cover to Sudoku later
          if (setMatrixRowData < matrixRowCounter) {
            rowsData[matrixRowCounter] =
                IdPacking::packId(boardI, boardJ, unpackedDigit, Sudo::MAX_DIGIT, Sudo::MAX_DIGIT, Sudo::MAX_DIGIT);
            setMatrixRowData++;
          }
        }
        matrixColumnBase += constraints[constraintIndex]->getDlxConstraintColumnsAmount();
      }
      matrixRowCounter++;
    }
  }

  SparseCoordinateMatrix sparseMatrix(matrix, secondaryColumns, true, rowsData);
  return sparseMatrix;
}

void Solver::reduceExactCoverSolutionToSudokuSolution(std::vector<std::vector<Sudo::Digit>>& board,
                                                      const SparseCoordinateMatrix& matrix,
                                                      const std::unordered_set<int32_t>& solutionRows) {
  for (const auto& rowIndex : solutionRows) {
    int32_t packedData = matrix.getRowData(rowIndex);
    // Uses the same method used to identify the cells of the SUDOKU_CELL constraint, but the process here is reversed.
    // This was set when reducing Sudoku to Exact Cover
    const auto [boardRow, boardColumn, digit] =
        IdPacking::unpackId(packedData, Sudo::MAX_DIGIT, Sudo::MAX_DIGIT, Sudo::MAX_DIGIT);
    board[boardRow][boardColumn] = static_cast<Sudo::Digit>(digit + 1);
  }
}
