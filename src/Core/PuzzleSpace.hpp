#pragma once

#include "Digits.hpp"
#include "Index.hpp"

/** A possibility for a cell in a Puzzle
 */
struct PuzzleSpace {
  /** Constructor
   */
  constexpr PuzzleSpace(Index rows, Index columns, Digit digits)
      : rowsCount(rows)
      , columnsCount(columns)
      , digitsCount(digits) {}

  /** The amount of rows available in the puzzle grid
   */
  const Index rowsCount = 0;

  /** The amount of columns available in the puzzle grid
   */
  const Index columnsCount = 0;

  /** The amount of digits that each cell in the puzzle can have
   */
  const Digit digitsCount = 0;
};
