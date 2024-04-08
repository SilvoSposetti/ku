#include "Solver.h"

#include "../Sudo.h"
#include "../Validator.h"
#include "../utilities/IdPacking.h"
#include "AlgorithmX.h"


std::vector<std::vector<Sudo::Digit>>
Solver::createNewBoard(const std::vector<std::unique_ptr<AbstractConstraint>>& constraints,
                       std::optional<int32_t> seed) {

  std::vector<std::vector<Sudo::Digit>> newField = Sudo::emptyField();
  std::vector<std::vector<bool>> givenMask = Sudo::emptyGivenMask();

  const bool created = Solver::solve(newField, constraints, false, seed);

  if (!created) {
    std::string constraintsNames;
    for (const auto& constraint : constraints) {
      constraintsNames += constraint->getName() + ", ";
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
  for (const auto& constraint : constraints) {
    if (constraint->getPrimaryItemsAmount() > 0) {
      return true;
    }
  }
  return false;
}

bool Solver::solve(std::vector<std::vector<Sudo::Digit>>& board,
                   const std::vector<std::unique_ptr<AbstractConstraint>>& constraints,
                   bool checkForUniqueness,
                   std::optional<int32_t> seed) {

  // // TODO: Check that the constraints create a valid matrix
  // if (!matrix.isSolvableByAlgorithmX()) {
  //   return false;
  // }

  // Reduce problem: Sudoku -> Exact Cover
  // SparseCoordinateMatrix matrix = reduceSudokuProblemToExactCoverProblem(board, constraints);
  DataStructure dataStructure = DataStructure(board, constraints);

  // No need to reduce the solution back to a valid board when simply checking for uniqueness
  if (checkForUniqueness) {
    return AlgorithmX::hasUniqueSolution(dataStructure, seed);
  }
  // AlgorithmX::printDataStructure(matrix);

  // Find a possible solution
  std::unordered_set<int32_t> solution = AlgorithmX::findOneSolution(dataStructure, seed);

  // Use first solution out of all those that are found
  // This solution should have 81 elements, one for each cell piked
  if (solution.size() == Sudo::TOTAL_DIGITS) {
    // Reduce solution: Exact Cover -> Sudoku
    reduceExactCoverSolutionToSudokuSolution(board, dataStructure, solution);

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
    totalColumns += constraint->getPrimaryItemsAmount() + constraint->getSecondaryItemsAmount();
  }

  // Set up constraints cache, containing first primary items amount and then the items, then secondary items amount and
  // then the items
  std::vector<std::pair<int32_t, std::vector<std::vector<int32_t>>>> primaryCache{constraints.size()};
  std::vector<std::pair<int32_t, std::vector<std::vector<int32_t>>>> secondaryCache{constraints.size()};
  {
    int32_t constraintCounter = 0;
    // First primary items
    for (const auto& constraint : constraints) {
      primaryCache[constraintCounter] =
          std::make_pair(constraint->getPrimaryItemsAmount(), constraint->getPrimaryItems());
      secondaryCache[constraintCounter] =
          std::make_pair(constraint->getSecondaryItemsAmount(), constraint->getSecondaryItems());
      ;
      constraintCounter++;
    }
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
      // First: primary items
      for (const auto& [primaryAmount, primaryItems] : primaryCache) {
        if (primaryAmount > 0) {
          for (const auto& itemId : primaryItems[optionId]) {
            const int matrixColumnId = matrixColumnBase + itemId;
            // Set cell
            matrix[matrixRowCounter][matrixColumnId] = true;
            // Store row data in the matrix, this is used to reduce Exact Cover to Sudoku later
            if (setMatrixRowData < matrixRowCounter) {
              rowsData[matrixRowCounter] =
                  IdPacking::packId(boardI, boardJ, unpackedDigit, Sudo::MAX_DIGIT, Sudo::MAX_DIGIT, Sudo::MAX_DIGIT);
              setMatrixRowData++;
            }
          }
        }
        matrixColumnBase += primaryAmount;
      }
      // Then, secondary
      for (const auto& [secondaryAmount, secondaryItems] : secondaryCache) {
        if (secondaryAmount > 0) {

          for (const auto& itemId : secondaryItems[optionId]) {
            const int matrixColumnId = matrixColumnBase + itemId;
            // Set cell
            matrix[matrixRowCounter][matrixColumnId] = true;
            secondaryColumns.insert(matrixColumnId);
            // Store row data in the matrix, this is used to reduce Exact Cover to Sudoku later
            if (setMatrixRowData < matrixRowCounter) {
              rowsData[matrixRowCounter] =
                  IdPacking::packId(boardI, boardJ, unpackedDigit, Sudo::MAX_DIGIT, Sudo::MAX_DIGIT, Sudo::MAX_DIGIT);
              setMatrixRowData++;
            }
          }
        }
        matrixColumnBase += secondaryAmount;
      }
      matrixRowCounter++;
    }
  }

  SparseCoordinateMatrix sparseMatrix(matrix, secondaryColumns, true, rowsData);
  return sparseMatrix;
}

void Solver::reduceExactCoverSolutionToSudokuSolution(std::vector<std::vector<Sudo::Digit>>& board,
                                                      const DataStructure& dataStructure,
                                                      const std::unordered_set<int32_t>& solutionRows) {

  const auto& optionsData = dataStructure.getOptionsData();
  for (const auto& rowIndex : solutionRows) {
    const auto& OptionData = optionsData[rowIndex];
    board[OptionData.indexI][OptionData.indexJ] = OptionData.digit;
  }
}
