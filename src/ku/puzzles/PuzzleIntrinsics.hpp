#pragma once

#include "../utilities/ArrayUtilities.hpp"
#include "../utilities/MathUtilities.hpp"
#include "Cell.hpp"
#include "Digits.hpp"
#include "Grid.hpp"
#include "OptionId.hpp"
#include "Possibilities.hpp"
#include "PuzzleSpace.hpp"

#include <array>

/** The base 'constexpr' class for puzzle intrinsics where a single digit goes in each cell.
 * Provides constant members, which are initialized at compile-time.
 * @tparam rowsCount The amount of rows in the grid.
 * @tparam columnsCount The amount of columns in the grid.
 * @tparam digitsCount The amount of digits possible in each cell of the grid.
 */
template <PuzzleSpace puzzleSpace>
class PuzzleIntrinsics {
public:
  /** Constructor
   */
  constexpr PuzzleIntrinsics()
      : rows(puzzleSpace.rowsCount)
      , columns(puzzleSpace.columnsCount)
      , rowIndices(ArrayUtilities::createIotaArray<Index, puzzleSpace.rowsCount>(0))
      , columnIndices(ArrayUtilities::createIotaArray<Index, puzzleSpace.columnsCount>(0))
      , digits(Digits::createDigits<puzzleSpace.digitsCount>())
      , emptyGrid(
            ArrayUtilities::create2DArray<Digit, puzzleSpace.columnsCount, puzzleSpace.rowsCount>(Digits::invalidDigit))
      , allPossibilities(createPossibilities(rowIndices, columnIndices, digits)) {};

  /** Checks if a row index is valid for the puzzle
   * @param rowIndex The index to check
   * @return Whether the index is valid
   */
  constexpr bool isValidRowIndex(Index rowIndex) const {
    return std::ranges::contains(rowIndices, rowIndex);
  };

  /** Checks if a column index is valid for the puzzle
   * @param columnIndex The index to check
   * @return Whether the index is valid
   */

  constexpr bool isValidColumnIndex(Index columnIndex) const {
    return std::ranges::contains(columnIndices, columnIndex);
  };

  /** Checks if a digit is valid for the puzzle
   * @param digit The digit to check
   * @return Whether the digit is valid
   */
  constexpr bool isValidDigit(Digit digit) const {
    return Digits::isValid(digit) && std::ranges::contains(digits, digit);
  }

  /** Checks if a Cell is valid for the puzzle
   * @param cell The cell to check
   * @return Whether the cell is valid
   */
  constexpr bool isValidCell(const Cell& cell) const {
    return isValidRowIndex(cell.rowIndex) && isValidColumnIndex(cell.columnIndex) && isValidDigit(cell.digit);
  }

  /** Checks if a puzzle is compatible with classic Sudoku
   * @return Whether the puzzle is compatible with classic Sudoku
   */
  constexpr bool isSudoku() const {
    return rows == 9 && columns == 9 && digits.size() == 9;
  }

  /** Checks if a puzzle is a square
   * @return Whether the puzzle is a square
   */
  constexpr bool isSquare() const {
    return rows == columns;
  }

  /** Checks if a cell defined by a row and column indices are on the positive diagonal of a square puzzle.
   * The positive diagonal is the diagonal going from the bottom-left to the top-right of the puzzle.
   * If the puzzle is not a square, it cannot have a positive diagonal.
   * @param rowIndex The row index
   * @param columnIndex The column index
   * @return Whether the row and column indices describe a square on the positive diagonal.
   */
  constexpr bool isOnPositiveDiagonal(Index rowIndex, Index columnIndex) const {
    if (isSquare()) {
      if (isValidRowIndex(rowIndex) && isValidColumnIndex(columnIndex)) {
        return rowIndex + columnIndex == (rowIndices.back()); // or also puzzle.columnIndices.back()
      }
    }
    return false;
  }

  /** Checks if a cell defined by a row and column indices are on the negative diagonal of a square puzzle.
   * The negative diagonal is the diagonal going from the top-left to the bottom-right of the puzzle.
   * If the puzzle is not a square, it cannot have a negative diagonal.
   * @param rowIndex The row index
   * @param columnIndex The column index
   * @return Whether the row and column indices describe a square on the negative diagonal.
   */
  constexpr bool isOnNegativeDiagonal(Index rowIndex, Index columnIndex) const {
    if (isSquare()) {
      if (isValidRowIndex(rowIndex) && isValidColumnIndex(columnIndex)) {
        return rowIndex == columnIndex;
      }
    }
    return false;
  }

  /** Computes whether the puzzle has any even digits
   * @return Whether the puzzle has any even digits
   */
  constexpr bool hasEvenDigits() const {
    return std::ranges::any_of(digits, [](const auto& digit) { return Digits::isEven(digit); });
  }

  /** Computes whether the puzzle has any odd digits
   * @return Whether the puzzle has any odd digits
   */
  constexpr bool hasOddDigits() const {
    return std::ranges::any_of(digits, [](const auto& digit) { return Digits::isOdd(digit); });
  }

  /** Computes the ID of a cell. IDs start at 0 from the top-left, and increment for each cell in row major order.
   * @param rowIndex The row index
   * @param columnIndex The column index
   * @return If the location provided is valid, its cell ID.
   */
  constexpr std::optional<OptionId> computeCellId(Index rowIndex, Index columnIndex) const {
    if (!isValidRowIndex(rowIndex) || !isValidColumnIndex(columnIndex)) {
      return {};
    }
    return static_cast<OptionId>(rowIndex) * columns + columnIndex;
  }

  /** Finds the neighbor of a cell according to a reference cell and an offset from it.
   * @param rowIndex The row index
   * @param columnIndex The column index
   * @return If the reference cell and its computer neighbor are valid, then the computed neighbor.
   */
  constexpr std::optional<std::pair<Index, Index>>
  computeNeighbor(Index rowIndex, Index columnIndex, int32_t rowOffset, int32_t columnOffset) const {
    if (!isValidRowIndex(rowIndex) || !isValidColumnIndex(columnIndex)) {
      return {};
    }
    const auto newRow = static_cast<Index>(static_cast<int32_t>(rowIndex) + rowOffset);
    const auto newColumn = static_cast<Index>(static_cast<int32_t>(columnIndex) + columnOffset);
    if (!isValidRowIndex(newRow) || !isValidColumnIndex(newColumn)) {
      return {};
    }
    return std::make_pair(newRow, newColumn);
  }

  /** Finds the neighbor of a cell according to a reference cell and an offset from it. The neighbor can be found by
   * wrapping around the edges of the puzzle.
   * @param rowIndex The row index
   * @param columnIndex The column index
   * @return If the reference cell is valid, then the computed neighbor.
   */
  constexpr std::optional<std::pair<Index, Index>>
  computeNeighborTorus(Index rowIndex, Index columnIndex, int32_t rowOffset, int32_t columnOffset) const {
    if (!isValidRowIndex(rowIndex) || !isValidColumnIndex(columnIndex)) {
      return {};
    }
    return std::make_pair(static_cast<Index>(MathUtilities::modulo(static_cast<int32_t>(rowIndex) + rowOffset,
                                                                   static_cast<int32_t>(rows))),
                          static_cast<Index>(MathUtilities::modulo(static_cast<int32_t>(columnIndex) + columnOffset,
                                                                   static_cast<int32_t>(columns))));
  }

private:
  /** Constructs an array with an ordered set of possibilities covering all cells
   * @param rowIndices All the valid row indices, ordered.
   * @param columnIndices All the valid column indices, ordered.
   * @param digits All the digits, ordered.
   * @return From the top-left cell going right and then going down line by line, the ordered set of all possibilities
   * for every cell.
   */
  constexpr Possibilities<puzzleSpace> createPossibilities(std::array<Index, puzzleSpace.rowsCount> rowIndices,
                                                           std::array<Index, puzzleSpace.columnsCount> columnIndices,
                                                           std::array<Digit, puzzleSpace.digitsCount> digits) {
    auto array = Possibilities<puzzleSpace>();
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
  const Index rows = 0;
  /** The amount of columns in the puzzle grid
   */
  const Index columns = 0;

  /** The ordered set of valid row indices, from 0 to the amount of rows - 1
   */
  const std::array<Index, puzzleSpace.rowsCount> rowIndices;

  /** The ordered set of valid column indices, from 0 to the amount of columns - 1
   */
  const std::array<Index, puzzleSpace.columnsCount> columnIndices;

  /** The ordered set of valid digits
   */
  const std::array<Digit, puzzleSpace.digitsCount> digits;

  /** A 2D matrix of the empty grid, with only invalid digits
   */
  const Grid<puzzleSpace> emptyGrid;

  /** The canonical ordering of all the possibilities in the puzzle space
   */
  const Possibilities<puzzleSpace> allPossibilities;
};