#include "Setter.h"

#include "MaskUtilities.h"
#include "solver/Solver.h"
#include "utilities/Timer.h"

std::unique_ptr<Board> Setter::generate(int32_t totalDigits,
                                        SymmetryType symmetryType,
                                        const std::vector<std::unique_ptr<AbstractConstraint>>& constraints,
                                        std::optional<int32_t> seed) {

  Timer timer;
  // Create new random solution once
  const std::vector<std::vector<Sudo::Digit>> randomSolution = Solver::createNewBoard(constraints, seed);
  timer.printElapsed("Solution generated       ");

  // Skip mask generation if there are no missing digits
  if (totalDigits == Sudo::TOTAL_DIGITS) {
    return std::make_unique<Board>(randomSolution, Sudo::fullGivenMask());
  }

  // Try out multiple given masks until one makes the Sudoku unique
  constexpr int32_t totalTries = 100;
  int32_t counter = 0;
  std::vector<std::vector<bool>> givenMask;

  std::shared_ptr<RandomGenerator> randomGenerator = std::make_shared<RandomGenerator>(seed);
  timer.reset();
  while (counter < totalTries) {
    ++counter;

    // Create given mask
    if (symmetryType == SymmetryType::ONE_DIAGONAL_MIRROR)
      givenMask = MaskUtilities::diagonalMirrorMask(totalDigits, randomGenerator);
    else if (symmetryType == SymmetryType::ONE_DIAGONAL_ROTATION)
      givenMask = MaskUtilities::diagonalRotationMask(totalDigits, randomGenerator);
    //    else if (symmetryType == SymmetryType::TWO_DIAGONALS_MIRROR) givenMask = randomMask(totalDigits);
    //    else if (symmetryType == SymmetryType::TWO_DIAGONALS_ROTATION) givenMask = randomMask(totalDigits);
    //    else if (symmetryType == SymmetryType::ONE_AXIS_MIRROR) givenMask = randomMask(totalDigits);
    //    else if (symmetryType == SymmetryType::ONE_AXIS_ROTATION) givenMask = randomMask(totalDigits);
    //    else if (symmetryType == SymmetryType::TWO_AXES_MIRROR) givenMask = randomMask(totalDigits);
    //    else if (symmetryType == SymmetryType::TWO_AXES_ROTATION) givenMask = randomMask(totalDigits);
    else
      givenMask = MaskUtilities::randomMask(totalDigits, randomGenerator);

    // Create board to use for solving
    std::vector<std::vector<Sudo::Digit>> field = randomSolution;
    for (const auto& i : Sudo::INDICES) {
      for (const auto& j : Sudo::INDICES) {
        if (!givenMask[i][j]) {
          field[i][j] = Sudo::Digit::NONE;
        }
      }
    }

    if (Solver::isUnique(field, constraints)) {
      timer.printElapsed("Unique board generated   ");
      return std::make_unique<Board>(randomSolution, givenMask);
    }
  }
  std::cout << "Unable to create board after " << counter << " tries" << std::endl;
  return std::make_unique<Board>(randomSolution, Sudo::emptyGivenMask());
}

std::unique_ptr<Board> Setter::generate(const std::vector<std::vector<Sudo::Digit>>& givens,
                                        const std::vector<std::unique_ptr<AbstractConstraint>>& constraints,
                                        std::optional<int32_t> seed) {
  const auto solution = Solver::fillExistingBoard(givens, constraints, seed);
  if (solution != Sudo::emptyField()) {
    std::vector<std::vector<bool>> givenMask = Sudo::emptyGivenMask();
    for (const auto& i : Sudo::INDICES) {
      for (const auto& j : Sudo::INDICES) {
        if (!givenMask[i][j]) {
          givenMask[i][j] = givens[i][j] != Sudo::Digit::NONE;
        }
      }
    }
    return std::make_unique<Board>(solution, givenMask);
  }
  
  std::cout << "Unable to find solution with provided givens" << std::endl;
  return std::make_unique<Board>(Solver::createNewBoard(constraints, seed), Sudo::emptyGivenMask());
}
