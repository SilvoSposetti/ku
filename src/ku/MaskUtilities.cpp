#include "MaskUtilities.h"

#include "Sudo.h"

bool MaskUtilities::isOnMainDiagonal(int32_t rowIndex, int32_t columnIndex) {
  return rowIndex == columnIndex;
}

bool MaskUtilities::isOnCenter(int32_t rowIndex, int32_t columnIndex) {
  return rowIndex == Sudo::MID_INDEX && columnIndex == Sudo::MID_INDEX;
}

std::vector<std::vector<bool>> MaskUtilities::randomMask(int32_t totalDigits,
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

std::vector<std::vector<bool>> MaskUtilities::diagonalMirrorMask(int32_t totalDigits,
                                                                 std::shared_ptr<RandomGenerator> randomGenerator) {
  std::vector<std::vector<bool>> mask = Sudo::fullGivenMask();
  int32_t digitsToRemove = Sudo::TOTAL_DIGITS - totalDigits;
  // "Diagonal" digits are removed once at the beginning, then in pairs
  int32_t diagonalDigitsRemaining = 9;
  // "Other" ("Non-Diagonal") are always removed in pairs for symmetry
  int32_t otherDigitsRemaining = 72;

  if (digitsToRemove % 2 != 0) {
    const int32_t randomIndex = randomGenerator->uniformInteger(Sudo::MIN_INDEX, Sudo::MAX_INDEX);
    mask[randomIndex][randomIndex] = false;
    digitsToRemove--;
    diagonalDigitsRemaining--;
  }

  while (digitsToRemove > 0) {
    // Randomly decide if two digits need to be removed either from the diagonal or from the other ones
    const int32_t random = randomGenerator->uniformInteger(2, diagonalDigitsRemaining + otherDigitsRemaining);
    if (random < diagonalDigitsRemaining) {
      // Remove two digits from the diagonal
      const int32_t index1 = randomGenerator->uniformInteger(Sudo::MIN_INDEX, Sudo::MAX_INDEX);
      const int32_t index2 = randomGenerator->uniformInteger(Sudo::MIN_INDEX, Sudo::MAX_INDEX);
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
      const int32_t i = randomGenerator->uniformInteger(Sudo::MIN_INDEX, Sudo::MAX_INDEX);
      const int32_t j = randomGenerator->uniformInteger(Sudo::MIN_INDEX, Sudo::MAX_INDEX);
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

std::vector<std::vector<bool>> MaskUtilities::diagonalRotationMask(int32_t totalDigits,
                                                                   std::shared_ptr<RandomGenerator> randomGenerator) {
  std::vector<std::vector<bool>> mask = Sudo::fullGivenMask();
  int32_t digitsToRemove = Sudo::TOTAL_DIGITS - totalDigits;

  // If amount of total digits to remove is not even: need to remove the center
  if (digitsToRemove % 2 != 0) {
    mask[4][4] = false;
    digitsToRemove--;
  }
  // Then, can remove digits two-by-to with diagonal rotation
  while (digitsToRemove > 0) {
    const int32_t i = randomGenerator->uniformInteger(Sudo::MIN_INDEX, Sudo::MAX_INDEX);
    const int32_t j = randomGenerator->uniformInteger(Sudo::MIN_INDEX, Sudo::MAX_INDEX);
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