#pragma once

#include "Sudo.h"

#include <string>

/** Stores the board of a Sudoku
 */
class Board {
public:
  /** Constructor
   * @param solution The solution
   * @param givenMask The given Mask
   */
  Board(const std::vector<std::vector<Sudo::Digit>>& solution, const std::vector<std::vector<bool>>& givenMask);
  
  /** Prints the given pattern of this board to console
   */
  void printGivenPattern() const;

  /** Prints the entire board to console
   */
  void print() const;

  /** Retrieves a reference to the solution of this board
   * @return The solution
   */
  const std::vector<std::vector<Sudo::Digit>>& getSolution() const;

  /** Retrieves a reference to the given mask of this board
   * @return The given mask
   */
  const std::vector<std::vector<bool>>& getGivenMask() const;

  /** Retrieves a computed field of the board
   * @return The field
   */
  std::vector<std::vector<Sudo::Digit>> getField() const;

private:
  /// The solution
  const std::vector<std::vector<Sudo::Digit>> solution;
  /// The given mask
  const std::vector<std::vector<bool>> givenMask;
};
