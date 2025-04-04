#pragma once

#include "../constraints/AbstractConstraint.hpp"
#include "../constraints/ConstraintFactory.hpp"
#include "../constraints/ConstraintType.hpp"
#include "../drawing/DrawingOptions.hpp"
#include "../drawing/PuzzleDrawing.hpp"
#include "PuzzleIntrinsics.hpp"

#include <filesystem>

/** The base class for grid-like puzzles where a single digit goes in each cell.
 * @tparam rowsCount The amount of rows in the grid.
 * @tparam columnsCount The amount of columns in the grid.
 * @tparam digitsCount The amount of digits possible in each cell of the grid.
 */
template <PuzzleSpace puzzleSpace>
class Puzzle : public PuzzleIntrinsics<puzzleSpace> {
public:
  constexpr Puzzle(const std::string& name,
                   const std::vector<Cell>& clues,
                   ConstraintType constraintTypes,
                   std::optional<int32_t> seed)
      : PuzzleIntrinsics<puzzleSpace>()
      , name(name)
      , seed(seed)
      , constraints(createConstraints(constraintTypes))
      , givenCells(getOnlyValidClues(clues))
      , grid(initializeGrid()) {};

  std::array<std::array<Digit, puzzleSpace.columnsCount>, puzzleSpace.rowsCount> initializeGrid() const {
    auto grid =
        ArrayUtilities::create2DArray<Digit, puzzleSpace.columnsCount, puzzleSpace.rowsCount>(Digits::invalidDigit);
    for (const auto& cell : givenCells) {
      grid[cell.rowIndex][cell.columnIndex] = cell.digit;
    }
    return grid;
  }

  /** Prints the puzzle grid to stdout
   */
  void printGrid() const {
    for (const auto& line : gridAsText()) {
      std::puts(line.c_str());
    }
  };

  /** Constructs the puzzle grid as a list of text lines.
   * @return The list of lines that represent the puzzle gird
   */
  std::vector<std::string> gridAsText() const {
    const auto createLine = [](const std::string& first,
                               const std::string& blank,
                               const std::string& last,
                               const std::vector<std::string>& elements) {
      auto line = first + blank;
      for (const auto& element : elements) {
        line += element + blank;
      }
      return line + last;
    };

    std::vector<std::string> lines;
    for (const auto& rowIndex : this->rowIndices) {
      if (rowIndex == this->rowIndices.front()) {
        const auto elements = std::vector<std::string>(this->columns, "━");
        lines.push_back(createLine("┏", "━", "┓", elements));
      }
      std::vector<std::string> digitStrings;
      std::ranges::transform(grid[rowIndex], std::back_inserter(digitStrings), [](const auto& digit) {
        return Digits::isValid(digit) ? std::to_string(digit) : "◌";
      });
      lines.push_back(createLine("┃", " ", "┃", digitStrings));
      if (rowIndex == this->rowIndices.back()) {
        const auto elements = std::vector<std::string>(this->columns, "━");
        lines.push_back(createLine("┗", "━", "┛", elements));
      }
    }
    return lines;
  }

  /** Generates and stores the puzzle board to an SVG file in the provided directory
   * @param location Where the file should be stored
   * @return whether storing was successufl
   */
  bool exportToSvg(const std::filesystem::path& location) const {
    const auto options = DrawingOptions(1000, 150, constraints.size());
    const auto document =
        PuzzleDrawing::create<puzzleSpace.rowsCount, puzzleSpace.columnsCount>(name, options, grid, constraints);
    return document->writeToFile(location);
  };

private:
  /** Constructs the list of constraints
   * @param constraintTypes A bitflag of the constraints
   * @return The list of constructed constraints
   */
  std::vector<std::unique_ptr<AbstractConstraint>> createConstraints(const ConstraintType constraintTypes) const {
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

  /** Selects only the valid clues out of the provided ones
   * @param clues A list of clues
   * @return The list of constructed constraints
   */
  std::vector<Cell> getOnlyValidClues(const std::vector<Cell>& clues) {
    std::vector<Cell> result;
    result.reserve(clues.size());
    for (const auto& cell : clues) {
      if (this->isCellValid(cell)) {
        result.push_back(cell);
      }
    }
    return result;
  }

public:
  ///  The name of the puzzle
  const std::string name;

  /// The seed for the random number generator
  const std::optional<int32_t> seed;

  /// The list of constraint on the Puzzle
  const std::vector<std::unique_ptr<AbstractConstraint>> constraints;

  /// The cells with given values
  std::vector<Cell> givenCells;

  /// A 2D matrix of the grid, intialized with invalid digits
  std::array<std::array<Digit, puzzleSpace.columnsCount>, puzzleSpace.rowsCount> grid = this->emptyGrid;
};