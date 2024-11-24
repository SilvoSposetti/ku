#pragma once

#include "Digits.hpp"
#include "utilities/ArrayUtilities.hpp"

#include <array>
#include <cstdint>

/** The base class for grid-like puzzles where a single digit goes in each cell
 */
template <uint8_t rowsCount, uint8_t columnsCount, uint8_t digitsCount>
class Puzzle {
public:
  /** Constructor
   */
  constexpr Puzzle()
      : rows(rowsCount)
      , columns(columnsCount)
      , rowIndices(ArrayUtilities::createIotaArray<uint8_t, rowsCount>(0))
      , columnIndices(ArrayUtilities::createIotaArray<uint8_t, columnsCount>(0))
      , digits(Digits::createDigits<digitsCount>())
      , grid(ArrayUtilities::create2DArray<Digit, columnsCount, rowsCount>(Digits::invalidDigit)) {};

public:
  /** The amount of rows in the puzzle grid
   */
  const uint8_t rows = 0;
  /** The amount of columns in the puzzle grid
   */
  const uint8_t columns = 0;

  /** The ordered set of valid row indices, from 0 to the amount of rows - 1
   */
  const std::array<uint8_t, rowsCount> rowIndices;
  /** The ordered set of valid column indices, from 0 to the amount of columns - 1
   */
  const std::array<uint8_t, columnsCount> columnIndices;

  /** The ordered set of valid digits
   */
  const std::array<Digit, digitsCount> digits;

  /** A 2D matrix of the empty grid, where all digits are invalid
   */
  const std::array<std::array<Digit, columnsCount>, rowsCount> grid;
};