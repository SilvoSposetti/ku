#pragma once

#include "utilities/Utilities.h"
#include "Board.h"
#include "constraints/AbstractConstraint.h"
/** Constructor
 * @param inSolution The solution
 * @param inGivenMask The given Mask
 */
class Setter {
public:
  Setter() = delete;

  /** Generates a random sudoku with the given inputs
   * @param totalDigits How many total digits it should have
   * @param symmetryType The pattern that should be used to create the given mask
   * @param constraints The set of constraints that are used to generate the solution
   * @return A randomly generated board
   */
  static std::unique_ptr<Board> generate(int32_t totalDigits,
                                         SymmetryType symmetryType,
                                         const std::vector<std::unique_ptr<AbstractConstraint>>& constraints);

private:
  /** Determines whether a cell is on the main diagonal
   * @param rowIndex The row index of the cell
   * @param columnIndex The column index of the cell
   * @return Whether the cell is on the main diagonal
   */
  static inline bool isOnMainDiagonal(int32_t rowIndex, int32_t columnIndex);

  /** Determines whether a cell is in the center of the board
   * @param rowIndex The row index of the cell
   * @param columnIndex The column index of the cell
   * @return Whether the cell is in the center of the board
   */
  static inline bool isOnCenter(int32_t rowIndex, int32_t columnIndex);

  /** Computes and returns a randomly generated mask according to a random pattern
   * @param totalDigits How many digits should be left on the board
   * @return The constructed given mask
   */
  static std::vector<std::vector<bool>> randomMask(int32_t totalDigits);

  /** Computes and returns a randomly generated mask according to a diagonal mirror pattern
   * @param totalDigits How many digits should be left on the board
   * @return The constructed given mask
   */
  static std::vector<std::vector<bool>> diagonalMirrorMask(int32_t totalDigits);

  /** Computes and returns a randomly generated mask according to a diagonal rotation pattern
   * @param totalDigits How many digits should be left on the board
   * @return The constructed given mask
   */
  static std::vector<std::vector<bool>> diagonalRotationMask(int32_t totalDigits);
};
