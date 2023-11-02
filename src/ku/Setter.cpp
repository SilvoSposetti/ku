#include "Setter.h"

#include "solver/Solver.h"
#include "utilities/Timer.h"

std::unique_ptr<Board> Setter::generate(int32_t totalDigits,
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
    // Try a random mask
    givenMask = Setter::randomMask(totalDigits, randomGenerator);

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

std::unique_ptr<Board> Setter::generate(const std::vector<std::vector<Sudo::Digit>>& clues,
                                        const std::vector<std::unique_ptr<AbstractConstraint>>& constraints,
                                        std::optional<int32_t> seed) {
  const auto solution = Solver::fillExistingBoard(clues, constraints, seed);
  if (solution != Sudo::emptyField()) {
    std::vector<std::vector<bool>> givenMask = Sudo::emptyGivenMask();
    for (const auto& i : Sudo::INDICES) {
      for (const auto& j : Sudo::INDICES) {
        if (!givenMask[i][j]) {
          givenMask[i][j] = clues[i][j] != Sudo::Digit::NONE;
        }
      }
    }
    return std::make_unique<Board>(solution, givenMask);
  }

  std::cout << "Unable to find solution with provided clues" << std::endl;
  return std::make_unique<Board>(Solver::createNewBoard(constraints, seed), Sudo::emptyGivenMask());
}

std::vector<std::vector<bool>> Setter::randomMask(int32_t totalDigits,
                                                  std::shared_ptr<RandomGenerator> randomGenerator) {
  std::vector<std::vector<bool>> mask = Sudo::fullGivenMask();
  int32_t digits = Sudo::TOTAL_DIGITS;
  while (digits != totalDigits) {
    const int32_t i = randomGenerator->uniformInteger(Sudo::MIN_INDEX, Sudo::MAX_INDEX);
    const int32_t j = randomGenerator->uniformInteger(Sudo::MIN_INDEX, Sudo::MAX_INDEX);
    if (mask[i][j]) {
      mask[i][j] = false;
      digits--;
    }
  }
  return mask;
}