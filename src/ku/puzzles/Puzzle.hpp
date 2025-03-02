#pragma once

#include "../constraints/AbstractConstraint.hpp"
#include "../constraints/ConstraintFactory.hpp"
#include "../constraints/ConstraintType.hpp"
#include "PuzzleIntrinsics.hpp"

/** The base class for grid-like puzzles where a single digit goes in each cell.
 * @tparam rowsCount The amount of rows in the grid.
 * @tparam columnsCount The amount of columns in the grid.
 * @tparam digitsCount The amount of digits possible in each cell of the grid.
 */
template <uint8_t rowsCount, uint8_t columnsCount, uint8_t digitsCount>
class Puzzle : public PuzzleIntrinsics<rowsCount, columnsCount, digitsCount> {
public:
  constexpr Puzzle(const std::string& name,
                   const std::vector<Cell>& clues,
                   ConstraintType constraintTypes,
                   std::optional<int32_t> seed)
      : PuzzleIntrinsics<rowsCount, columnsCount, digitsCount>()
      , name(name)
      , seed(seed)
      , constraints(getConstraintsList(constraintTypes))
      , grid(initializeGrid(clues)) {};

  std::array<std::array<Digit, columnsCount>, rowsCount> initializeGrid(const std::vector<Cell>& clues) const {
    auto grid = ArrayUtilities::create2DArray<Digit, columnsCount, rowsCount>(Digits::invalidDigit);
    if (!clues.empty()) {
      for (const auto& cell : clues) {
        if (PuzzleIntrinsics<rowsCount, columnsCount, digitsCount>::isCellValid(cell)) {
          grid[cell.rowIndex][cell.columnIndex] = cell.digit;
        }
      }
    }
    return grid;
  }

  virtual void printGrid() const = 0;

private:
  std::vector<std::unique_ptr<AbstractConstraint>> getConstraintsList(const ConstraintType constraintTypes) {
    std::vector<std::unique_ptr<AbstractConstraint>> constraintList;

    // SUDOKU_CELL constraint is always present
    constraintList.emplace_back(ConstraintFactory::makeConstraint(ConstraintType::SUDOKU_CELL));

    for (int32_t bitToCheck = 0; bitToCheck < 64; bitToCheck++) {
      const uint64_t valueToCheck = static_cast<uint64_t>(1) << bitToCheck;
      if (static_cast<uint64_t>(constraintTypes) & valueToCheck) {
        const ConstraintType singleConstraint = static_cast<ConstraintType>(valueToCheck);
        if (singleConstraint != ConstraintType::SUDOKU_CELL) {
          constraintList.emplace_back(ConstraintFactory::makeConstraint(singleConstraint));
        }
      }
    }
    return constraintList;
  }

public:
  ///  The name of the puzzle
  const std::string name;

  /// The seed for the random number generator
  const std::optional<int32_t> seed;

  /// The list of constraint on the Puzzle
  const std::vector<std::unique_ptr<AbstractConstraint>> constraints;

  /// A 2D matrix of the grid, intialized with invalid digits
  std::array<std::array<Digit, columnsCount>, rowsCount> grid =
      ArrayUtilities::create2DArray<Digit, columnsCount, rowsCount>(Digits::invalidDigit);
};