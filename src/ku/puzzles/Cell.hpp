#pragma once

#include "Digits.hpp"
#include "Index.hpp"

#include <compare>

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
  constexpr Cell(Index rowIndex, Index columnIndex, Digit digit)
      : rowIndex(rowIndex)
      , columnIndex(columnIndex)
      , digit(digit) {};

  /** Default three-way operator<=>
   */
  std::strong_ordering operator<=>(const Cell&) const = default;

  bool isAtSameSpot(const Cell& other) const {
    return rowIndex == other.rowIndex && columnIndex == other.columnIndex;
  };

public:
  /** The row-index of the cell in the Puzzle
   */
  Index rowIndex = 0;

  /** The column-index of the cell in the Puzzle
   */
  Index columnIndex = 0;

  /** The digit that the cell could have
   */
  Digit digit = Digits::invalidDigit;
};

// Specialize std::hash for Cell, E.g. for using in std::unordered_set
namespace std {
template <>
struct hash<Cell> {
  size_t operator()(const Cell& cell) const {
    // Combine hashes of individual members
    size_t h1 = std::hash<Index>{}(cell.columnIndex);
    size_t h2 = std::hash<Index>{}(cell.rowIndex);
    size_t h3 = std::hash<Digit>{}(cell.digit);
    return h1 ^ (h2 << 1) ^ (h3 << 2);
  }
};
} // namespace std
