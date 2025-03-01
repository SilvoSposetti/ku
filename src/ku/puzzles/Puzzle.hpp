#pragma once

#include "PuzzleIntrinsics.hpp"

/** The base class for grid-like puzzles where a single digit goes in each cell.
 * @tparam rowsCount The amount of rows in the grid.
 * @tparam columnsCount The amount of columns in the grid.
 * @tparam digitsCount The amount of digits possible in each cell of the grid.
 */
template <uint8_t rowsCount, uint8_t columnsCount, uint8_t digitsCount>
class Puzzle : public PuzzleIntrinsics<rowsCount, columnsCount, digitsCount> {
public:
  constexpr Puzzle()
      : PuzzleIntrinsics<rowsCount, columnsCount, digitsCount>()
      , grid(ArrayUtilities::create2DArray<Digit, columnsCount, rowsCount>(Digits::invalidDigit)) {};

  virtual void printGrid() const = 0;

  /** A 2D matrix of the grid, intialized with invalid digits
   */
  std::array<std::array<Digit, columnsCount>, rowsCount> grid;
};