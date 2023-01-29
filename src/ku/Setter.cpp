#include "Setter.h"

#include "solver/Solver.h"

std::unique_ptr<Board> Setter::generate(int32_t totalDigits,
                                        SymmetryType symmetryType,
                                        const std::vector<std::unique_ptr<AbstractConstraint>>& constraints,
                                        std::shared_ptr<RandomGenerator> randomGenerator) {

  Timer timer;
  // Create new random solution once
  const std::vector<std::vector<Sudo>> randomSolution = Solver::createNewBoard(constraints, randomGenerator);
  timer.printElapsed("Solution generated       ");

  // Skip board generation if there are no missing digits
  if (totalDigits == TOTAL_DIGITS) {
    return std::make_unique<Board>(randomSolution, fullGivenMask());
  }

  // Try out multiple given masks until one makes the Sudoku unique
  constexpr int32_t totalTries = 100;
  int32_t counter = 0;
  std::vector<std::vector<bool>> givenMask;

  timer.reset();
  while (counter < totalTries) {
    ++counter;

    // Create given mask
    if (symmetryType == SymmetryType::ONE_DIAGONAL_MIRROR)
      givenMask = diagonalMirrorMask(totalDigits, randomGenerator);
    else if (symmetryType == SymmetryType::ONE_DIAGONAL_ROTATION)
      givenMask = diagonalRotationMask(totalDigits, randomGenerator);
    //    else if (symmetryType == SymmetryType::TWO_DIAGONALS_MIRROR) givenMask = randomMask(totalDigits);
    //    else if (symmetryType == SymmetryType::TWO_DIAGONALS_ROTATION) givenMask = randomMask(totalDigits);
    //    else if (symmetryType == SymmetryType::ONE_AXIS_MIRROR) givenMask = randomMask(totalDigits);
    //    else if (symmetryType == SymmetryType::ONE_AXIS_ROTATION) givenMask = randomMask(totalDigits);
    //    else if (symmetryType == SymmetryType::TWO_AXES_MIRROR) givenMask = randomMask(totalDigits);
    //    else if (symmetryType == SymmetryType::TWO_AXES_ROTATION) givenMask = randomMask(totalDigits);
    else
      givenMask = randomMask(totalDigits, randomGenerator);

    if (Solver::isUnique(randomSolution, givenMask, constraints)) {
      timer.printElapsed("Unique board generated   ");
      return std::make_unique<Board>(randomSolution, givenMask);
    }
  }
  std::cout << "Unable to create board after " << counter << " tries" << std::endl;
  return std::make_unique<Board>(randomSolution, emptyGivenMask());
}

std::vector<std::vector<bool>> Setter::randomMask(int32_t totalDigits,
                                                  std::shared_ptr<RandomGenerator> randomGenerator) {
  std::vector<std::vector<bool>> mask = fullGivenMask();
  int32_t digits = TOTAL_DIGITS;
  while (digits != totalDigits) {
    const int32_t i = randomGenerator->randomUniform(MIN_INDEX, MAX_INDEX);
    const int32_t j = randomGenerator->randomUniform(MIN_INDEX, MAX_INDEX);
    if (mask[i][j]) {
      mask[i][j] = false;
      digits--;
    }
  }
  return mask;
}

inline bool Setter::isOnMainDiagonal(int32_t rowIndex, int32_t columnIndex) {
  return rowIndex == columnIndex;
}

inline bool Setter::isOnCenter(int32_t rowIndex, int32_t columnIndex) {
  return rowIndex == MID_INDEX && columnIndex == MID_INDEX;
}

std::vector<std::vector<bool>> Setter::diagonalMirrorMask(int32_t totalDigits,
                                                          std::shared_ptr<RandomGenerator> randomGenerator) {
  std::vector<std::vector<bool>> mask = fullGivenMask();
  int32_t digitsToRemove = TOTAL_DIGITS - totalDigits;
  // "Diagonal" digits are removed once at the beginning, then in pairs
  int32_t diagonalDigitsRemaining = 9;
  // "Other" ("Non-Diagonal") are always removed in pairs for symmetry
  int32_t otherDigitsRemaining = 72;

  if (digitsToRemove % 2 != 0) {
    const int32_t randomIndex = randomGenerator->randomUniform(MIN_INDEX, MAX_INDEX);
    mask[randomIndex][randomIndex] = false;
    digitsToRemove--;
    diagonalDigitsRemaining--;
  }

  while (digitsToRemove > 0) {
    // Randomly decide if two digits need to be removed either from the diagonal or from the other ones
    const int32_t random = randomGenerator->randomUniform(2, diagonalDigitsRemaining + otherDigitsRemaining);
    if (random < diagonalDigitsRemaining) {
      // Remove two digits from the diagonal
      const int32_t index1 = randomGenerator->randomUniform(MIN_INDEX, MAX_INDEX);
      const int32_t index2 = randomGenerator->randomUniform(MIN_INDEX, MAX_INDEX);
      if (index1 != index2) {
        if (mask[index1][index1] && mask[index2][index2]) {
          mask[index1][index1] = false;
          digitsToRemove--;
          diagonalDigitsRemaining--;
          mask[index2][index2] = false;
          digitsToRemove--;
          diagonalDigitsRemaining--;
        }
      }
    } else {
      // Remove two digits from the other digits
      const int32_t i = randomGenerator->randomUniform(MIN_INDEX, MAX_INDEX);
      const int32_t j = randomGenerator->randomUniform(MIN_INDEX, MAX_INDEX);
      if (!isOnMainDiagonal(i, j) && mask[i][j]) {
        mask[i][j] = false;
        digitsToRemove--;
        otherDigitsRemaining--;
        const int32_t k = j;
        const int32_t l = i;
        mask[k][l] = false;
        digitsToRemove--;
        otherDigitsRemaining--;
      }
    }
  }
  return mask;
}

std::vector<std::vector<bool>> Setter::diagonalRotationMask(int32_t totalDigits,
                                                            std::shared_ptr<RandomGenerator> randomGenerator) {
  std::vector<std::vector<bool>> mask = fullGivenMask();
  int32_t digitsToRemove = TOTAL_DIGITS - totalDigits;

  // If amount of total digits to remove is not even: need to remove the center
  if (digitsToRemove % 2 != 0) {
    mask[4][4] = false;
    digitsToRemove--;
  }
  // Then, can remove digits two-by-to with diagonal rotation
  while (digitsToRemove > 0) {
    const int32_t i = randomGenerator->randomUniform(MIN_INDEX, MAX_INDEX);
    const int32_t j = randomGenerator->randomUniform(MIN_INDEX, MAX_INDEX);
    if (!isOnCenter(i, j) && digitsToRemove >= 2) {
      if (mask[i][j]) {
        mask[i][j] = false;
        digitsToRemove--;
        const int32_t k = 8 - i;
        const int32_t l = 8 - j;
        mask[k][l] = false;
        digitsToRemove--;
      }
    }
  }
  return mask;
}
