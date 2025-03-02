#pragma once

#include "../utilities/ArrayUtilities.hpp"
#include "Cell.hpp"
#include "Digits.hpp"

#include <array>
#include <cstdint>

/** The base 'constexpr' class for puzzle intrinsics where a single digit goes in each cell.
 * Provides constant members, which are initialized at compile-time.
 * @tparam rowsCount The amount of rows in the grid.
 * @tparam columnsCount The amount of columns in the grid.
 * @tparam digitsCount The amount of digits possible in each cell of the grid.
 */
template <uint8_t rowsCount, uint8_t columnsCount, uint8_t digitsCount>
class PuzzleIntrinsics {
public:
  /** Constructor
   */
  constexpr PuzzleIntrinsics()
      : rows(rowsCount)
      , columns(columnsCount)
      , rowIndices(ArrayUtilities::createIotaArray<uint8_t, rowsCount>(0))
      , columnIndices(ArrayUtilities::createIotaArray<uint8_t, columnsCount>(0))
      , digits(Digits::createDigits<digitsCount>())
      , emptyGrid(ArrayUtilities::create2DArray<Digit, columnsCount, rowsCount>(Digits::invalidDigit))
      , possibilities(createPossibilities(rowIndices, columnIndices, digits)) {};

  /** Virtual destructor
   */
  virtual ~PuzzleIntrinsics() = default;

  /** Checks if a row index is valid for the puzzle
   * @param rowIndex The index to check
   * @return Whether the index is valid
   */
  constexpr bool isRowIndexValid(uint8_t rowIndex) const {
    return std::ranges::contains(rowIndices, rowIndex);
  };

  /** Checks if a column index is valid for the puzzle
   * @param columnIndex The index to check
   * @return Whether the index is valid
   */

  constexpr bool isColumnIndexValid(uint8_t columnIndex) const {
    return std::ranges::contains(columnIndices, columnIndex);
  };

  /** Checks if a digit is valid for the puzzle
   * @param digit The digit to check
   * @return Whether the digit is valid
   */
  constexpr bool isDigitValid(Digit digit) const {
    return Digits::isValid(digit) && std::ranges::contains(digits, digit);
    ;
  }

  /** Checks if a Cell is valid for the puzzle
   * @param cell The cell to check
   * @return Whether the cell is valid
   */
  constexpr bool isCellValid(const Cell& cell) const {
    return isRowIndexValid(cell.rowIndex) && isColumnIndexValid(cell.columnIndex) && isDigitValid(cell.digit);
  }

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

  /** A 2D matrix of the empty grid, with only invalid digits
   */
  const std::array<std::array<Digit, columnsCount>, rowsCount> emptyGrid;

  /** The canonical ordering of all the possibilities in the puzzle space
   */
  const std::array<Cell, rowsCount * columnsCount * digitsCount> possibilities;
};