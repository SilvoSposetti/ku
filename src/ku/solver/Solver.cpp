#include "Solver.h"

#include "../Sudo.h"
#include "../Validator.h"
#include "../randomGenerator/RandomGenerator.h"
#include "../utilities/IdPacking.h"
#include "AlgorithmX.h"

#include <map>

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

void Solver::reorderColumns(SparseCoordinateMatrix& matrix, std::shared_ptr<RandomGenerator> randomGenerator) {
  const int32_t columnsAmount = matrix.getColumnsAmount();

  // Gather column information, in the form of lists of {columnIndex, columnSize} for primary and secondary items
  std::vector<std::pair<int32_t, int32_t>> primaryColumns;
  std::vector<std::pair<int32_t, int32_t>> secondaryColumns;
  for (int32_t i = 0; i < columnsAmount; i++) {
    const int32_t columnSize = matrix.getColumnValidCellsAmount(i);
    if (matrix.isColumnPrimary(i)) {
      primaryColumns.emplace_back(std::make_pair(i, columnSize));
    } else {
      secondaryColumns.emplace_back(std::make_pair(i, columnSize));
    }
  }

  // Sort both lists by the sizes
  const auto sortByColumnSize = [&](const std::pair<int32_t, int32_t>& left,
                                    const std::pair<int32_t, int32_t>& right) -> bool {
    return left.second < right.second;
  };
  // Sort primary columns by their size
  std::sort(primaryColumns.begin(), primaryColumns.end(), sortByColumnSize);

  // Sort secondary columns by their size as well, even though it doesn't seem to make a difference
  std::sort(secondaryColumns.begin(), secondaryColumns.end(), sortByColumnSize);

  // If the random generator is available, scramble equally-sized primary columns
  if (randomGenerator) {
    // Create map from all group sizes tp all the column indices that they contain
    std::map<int32_t, std::vector<int32_t>> map;
    for (const auto& [index, size] : primaryColumns) {
      if (map.find(size) == map.end()) {
        map.insert({size, std::vector<int32_t>({index})});
      } else {
        map.at(size).emplace_back(index);
      }
    }

    for (auto& [size, indices] : map) {
      indices = randomGenerator->randomShuffle(indices);
    }

    primaryColumns.clear();
    for (const auto& [size, indices] : map) {
      for (const auto& index : indices) {
        primaryColumns.emplace_back(std::make_pair(index, size));
      }
    }
  }

  // Create permutation vector
  std::vector<int32_t> permutation;
  for (const auto& p : primaryColumns) {
    permutation.emplace_back(p.first);
  }
  for (const auto& s : secondaryColumns) {
    permutation.emplace_back(s.first);
  }

  // Do matrix preprocessing
  matrix.reorderColumns(permutation);
}

bool Solver::solve(std::vector<std::vector<Sudo::Digit>>& board,
                   const std::vector<std::unique_ptr<AbstractConstraint>>& constraints,
                   bool checkForUniqueness,
                   std::shared_ptr<RandomGenerator> randomGenerator) {
  // Reduce problem: Sudoku -> Exact Cover
  SparseCoordinateMatrix matrix = reduceSudokuProblemToExactCoverProblem(board, constraints, randomGenerator);

  // Preprocess the matrix to get it ready for Algorithm X
  // matrix.preprocess(randomGenerator);

  // Check that the matrix is valid
  if (!matrix.isSolvableByAlgorithmX()) {
    return false;
  }

  // Sort columns increasing by size for (on average) faster solve, and (if necessary) scramble equally-sized columns
  // from the result
  reorderColumns(matrix, randomGenerator);

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
  constexpr int32_t maximumRows = Sudo::MAX_DIGIT * Sudo::MAX_DIGIT * Sudo::MAX_DIGIT;
  // Each given reduces the amount of rows by (Sudo::MAX_DIGIT - 1)
  const int32_t totalRows = maximumRows - (Sudo::MAX_DIGIT - 1) * givenAmount;

  int32_t totalColumns = 0;
  for (const auto& constraint : constraints) {
    totalColumns += constraint->getDlxConstraintColumnsAmount();
  }

  // Initialize matrix with correct size
  std::vector<std::vector<bool>> matrix(totalRows, std::vector<bool>(totalColumns, false));
  std::vector<int32_t> rowsData(totalRows, -1);
  std::unordered_set<int32_t> secondaryColumns;

  // Randomize the sequence of digits that is passed when constructing the matrix or not
  const std::vector<Sudo::Digit> digitsSequence =
      randomGenerator ? randomGenerator->randomShuffle(Sudo::SUDO_DIGITS) : Sudo::SUDO_DIGITS;

  int32_t matrixRowCounter = 0;
  int32_t matrixColumnCounter = 0;
  int32_t setMatrixRowData = -1;
  for (const auto& boardI : Sudo::INDICES) { // Go through all sudoku rows
    for (const auto& boardJ : Sudo::INDICES) { // Go through all sudoku columns
      const Sudo::Digit actualDigit = board[boardI][boardJ];
      for (const auto& possibleDigit : digitsSequence) { // Go through all possible digits for this cell
        // Avoid rows where the cell and its possible digits don't create a "1" in the matrix, since the digit
        // is already given
        if (actualDigit == Sudo::Digit::NONE || actualDigit == possibleDigit) {
          for (const auto& constraint : constraints) {
            for (int32_t columnId = 0; columnId < constraint->getDlxConstraintColumnsAmount(); ++columnId) {
              if (!constraint->isColumnPrimary(columnId)) {
                secondaryColumns.insert(matrixColumnCounter);
              }
              if (constraint->getDlxConstraint(possibleDigit, boardI, boardJ, columnId)) {
                // Set cell
                matrix[matrixRowCounter][matrixColumnCounter] = true;
                // Store row data in the matrix, this is used to reduce Exact Cover to Sudoku later
                if (setMatrixRowData < matrixRowCounter) {
                  rowsData[matrixRowCounter] = IdPacking::packId(boardI,
                                                                 boardJ,
                                                                 static_cast<int32_t>(possibleDigit) - 1,
                                                                 Sudo::MAX_DIGIT,
                                                                 Sudo::MAX_DIGIT,
                                                                 Sudo::MAX_DIGIT);
                  setMatrixRowData++;
                }
              }
              matrixColumnCounter = (matrixColumnCounter + 1) % totalColumns;
            }
          }
          matrixRowCounter++;
        }
      }
    }
  }

  SparseCoordinateMatrix sparseMatrix(matrix, secondaryColumns, rowsData);
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
