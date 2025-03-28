#pragma once

#include "Digits.hpp"

/** A possibility for a cell in a Puzzle
 */
struct Cell {

  /** Default constructor
   */
  constexpr Cell() = default;

  /** Constructor
   * @param rowIndex The row-index of the cell in the Puzzle
   * @param columnIndex The column-index of the cell in the Puzzle
   * @param digit The digit that the cell could have
   */
  constexpr Cell(uint8_t rowIndex, uint8_t columnIndex, Digit digit)
      : rowIndex(rowIndex)
      , columnIndex(columnIndex)
      , digit(digit) {};

public:
  /** The row-index of the cell in the Puzzle
   */
  uint8_t rowIndex = 0;

  /** The column-index of the cell in the Puzzle
   */
  uint8_t columnIndex = 0;

  /** The digit that the cell could have
   */
  Digit digit = Digits::invalidDigit;
};
