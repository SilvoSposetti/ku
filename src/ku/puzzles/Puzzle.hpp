#pragma once

#include "Digits.hpp"
#include "Cell.hpp"
#include "utilities/ArrayUtilities.hpp"

#include <array>
#include <cstdint>

/** The base class for grid-like puzzles where a single digit goes in each cell.
 * @tparam rowsCount The amount of rows in the grid.
 * @tparam columnsCount The amount of columns in the grid.
 * @tparam digitsCount The amount of digits possible in each cell of the grid.
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
      , grid(ArrayUtilities::create2DArray<Digit, columnsCount, rowsCount>(Digits::invalidDigit))
      , possibilities(createPossibilities(rowIndices, columnIndices, digits)) {};

private:
  /** Constructs an array with an ordered set of possibilities covering all cells
   * @param rowIndices All the valid row indices, ordered.
   * @param columnIndices All the valid column indices, ordered.
   * @param digits All the digits, ordered.
   * @return From the top-left cell going right and then going down line by line, the ordered set of all possibilities
   * for every cell.
   */
  constexpr std::array<Cell, rowsCount * columnsCount * digitsCount>
  createPossibilities(std::array<uint8_t, rowsCount> rowIndices,
                      std::array<uint8_t, columnsCount> columnIndices,
                      std::array<Digit, digitsCount> digits) {
    constexpr auto amount = std::size_t(rowsCount) * std::size_t(columnsCount) * std::size_t(digitsCount);
    auto array = std::array<Cell, amount>();
    std::size_t i = 0;
    // First order priority: the rows
    for (const auto& rowIndex : rowIndices) {
      // Second order priority: the columns
      for (const auto& columnIndex : columnIndices) {
        // Last order priority: the digits
        for (const auto& digit : digits) {
          array[i] = Cell(rowIndex, columnIndex, digit);
          i++;
        }
      }
    }
    return array;
  }

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

  /** The canonical ordering of all the possibilities in the puzzle space
   */
  const std::array<Cell, rowsCount * columnsCount * digitsCount> possibilities;
};