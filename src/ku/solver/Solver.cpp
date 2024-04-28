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

  // Reduce problem: Sudoku -> Exact Cover
  DataStructure dataStructure = DataStructure(board, constraints);

  if (!dataStructure.isPotentiallySolvableByAlgorithmX()) {
    return false;
  }

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

void Solver::reduceExactCoverSolutionToSudokuSolution(std::vector<std::vector<Sudo::Digit>>& board,
                                                      const DataStructure& dataStructure,
                                                      const std::unordered_set<int32_t>& solutionRows) {

  const auto& optionsData = dataStructure.getOptionsData();
  for (const auto& rowIndex : solutionRows) {
    const auto& OptionData = optionsData[rowIndex];
    board[OptionData.indexI][OptionData.indexJ] = OptionData.digit;
  }
}
