#pragma once
#include "randomGenerator/RandomGenerator.h"

#include <memory>

namespace MaskUtilities {
/** Determines whether a cell is on the main diagonal
 * @param rowIndex The row index of the cell
 * @param columnIndex The column index of the cell
 * @return Whether the cell is on the main diagonal
 */
bool isOnMainDiagonal(int32_t rowIndex, int32_t columnIndex);

/** Determines whether a cell is in the center of the board
 * @param rowIndex The row index of the cell
 * @param columnIndex The column index of the cell
 * @return Whether the cell is in the center of the board
 */
bool isOnCenter(int32_t rowIndex, int32_t columnIndex);

/** Computes and returns a randomly generated mask according to a random pattern
 * @param totalDigits How many digits should be left on the board
 * @param randomGenerator The random number generator instance used to draw random numbers
 * @return The constructed given mask
 */
std::vector<std::vector<bool>> randomMask(int32_t totalDigits, std::shared_ptr<RandomGenerator> randomGenerator);

/** Computes and returns a randomly generated mask according to a diagonal mirror pattern
 * @param totalDigits How many digits should be left on the board
 * @param randomGenerator The random number generator instance used to draw random numbers
 * @return The constructed given mask
 */
std::vector<std::vector<bool>> diagonalMirrorMask(int32_t totalDigits,
                                                  std::shared_ptr<RandomGenerator> randomGenerator);

/** Computes and returns a randomly generated mask according to a diagonal rotation pattern
 * @param totalDigits How many digits should be left on the board
 * @param randomGenerator The random number generator instance used to draw random numbers
 * @return The constructed given mask
 */
std::vector<std::vector<bool>> diagonalRotationMask(int32_t totalDigits,
                                                    std::shared_ptr<RandomGenerator> randomGenerator);
} // namespace MaskUtilities