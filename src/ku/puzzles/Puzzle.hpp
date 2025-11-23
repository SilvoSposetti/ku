#pragma once

#include "../constraintTemplated/ConstraintFactory.hpp"
#include "../drawing/DataStructureDrawing.hpp"
#include "../drawing/DrawingOptions.hpp"
#include "../drawing/PuzzleDrawing.hpp"
#include "../solver/AlgorithmC.hpp"
#include "../solver/DancingCellsStructure.hpp"
#include "../utilities/IdPacking.hpp"
#include "PuzzleIntrinsics.hpp"

#include <filesystem>
#include <iostream>
#include <ranges>
#include <unordered_set>
#include <vector>

/** The base class for grid-like puzzles where a single digit goes in each cell.
 * @tparam rowsCount The amount of rows in the grid.
 * @tparam columnsCount The amount of columns in the grid.
 * @tparam digitsCount The amount of digits possible in each cell of the grid.
 */
template <PuzzleSpace puzzleSpace>
class Puzzle : public PuzzleIntrinsics<puzzleSpace> {
public:
  constexpr Puzzle(const std::string& name,
                   const std::unordered_set<Cell>& clues,
                   ConstraintType constraintTypes,
                   std::optional<int32_t> seed)
      : PuzzleIntrinsics<puzzleSpace>()
      , name(name)
      , seed(seed)
      , constraints(createConstraints(constraintTypes))
      , givenCells(getOnlyValidClues(clues))
      , possibilities(constructActualPossibilities())
      , startingGrid(initializeGrid())
      , structure(createStructure())
      , solution(solve()) {};

  Grid<puzzleSpace> initializeGrid() const {
    auto grid = this->emptyGrid();
    for (const auto& cell : givenCells) {
      grid[cell.rowIndex][cell.columnIndex] = cell.digit;
    }
    return grid;
  }

  std::vector<Cell> constructActualPossibilities() const {
    const auto filter = [&](const Cell& possibility) {
      const auto found = std::ranges::find_if(
          this->givenCells, [&](const Cell& givenCell) { return possibility.isAtSameSpot(givenCell); });
      if (found != givenCells.end()) {
        const auto givenCell = *found;
        return possibility == givenCell;
      }
      return true;
    };
    auto filteredView = this->allPossibilities() | std::ranges::views::filter(filter);
    return std::vector<Cell>(filteredView.begin(), filteredView.end());
  };

  /** Prints the puzzle grid to stdout
   */
  void printGrid() const {
    for (const auto& line : gridAsText(startingGrid)) {
      std::puts(line.c_str());
    }
  };

  /** Prints the puzzle grid to stdout
   */
  void printSolution() const {
    for (const auto& line : gridAsText(solution)) {
      std::puts(line.c_str());
    }
  };

  /** Constructs the puzzle grid as a list of text lines.
   * @return The list of lines that represent the puzzle gird
   */
  std::vector<std::string> gridAsText(const Grid<puzzleSpace>& grid) const {
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
    const auto document = PuzzleDrawing::create<puzzleSpace.rowsCount, puzzleSpace.columnsCount>(
        name, options, startingGrid, constraints);
    return document->writeToFile(location);
  };

  /** Generates and stores the puzzle board to an SVG file in the provided directory
   * @param location Where the file should be stored
   * @return whether storing was successufl
   */
  bool exportDataStructureToSvg(const std::filesystem::path& location) const {

    const auto document =
        DataStructureDrawing::create<puzzleSpace>(name + "-ExactCover", structure, constraints, possibilities);
    document->writeToFile(location);
    return document->writeToFile(location);
    return true;
  };

  DancingCellsStructure createStructure() const {
    int32_t primaryItemsCount = 0;
    int32_t secondaryItemsCount = 0;
    auto idOffsets = std::vector<std::pair<int32_t, int32_t>>(constraints.size() + 1, {0, 0});
    {

      int32_t constraintIndex = 0;
      for (const auto& constraint : constraints) {
        idOffsets[constraintIndex + 1] = {idOffsets[constraintIndex].first + constraint->getPrimaryItemsAmount(),
                                          idOffsets[constraintIndex].second + constraint->getSecondaryItemsAmount()};
        constraintIndex++;
      }
      primaryItemsCount = idOffsets.back().first;
      secondaryItemsCount = idOffsets.back().second;
      for (auto& idOffset : idOffsets) {
        idOffset.second += idOffsets.back().first;
      }
    }

    auto optionsSpan = std::vector<std::pair<std::optional<OptionsSpan<PuzzleIntrinsics<puzzleSpace>{}>>,
                                             std::optional<OptionsSpan<PuzzleIntrinsics<puzzleSpace>{}>>>>();
    for (const auto& constraint : constraints) {
      optionsSpan.emplace_back(constraint->getPrimaryOptions(), constraint->getSecondaryOptions());
    }

    std::vector<std::vector<XccElement>> options;
    options.reserve(possibilities.size());
    for (const auto& [i, j, possibleDigit] : possibilities) {
      const int32_t globalOptionId = IdPacking::packId(
          i, j, possibleDigit - 1, puzzleSpace.rowsCount, puzzleSpace.columnsCount, puzzleSpace.digitsCount);
      std::vector<XccElement> option;
      option.reserve(computeOptionSize(optionsSpan, globalOptionId));
      int32_t constraintId = 0;
      for (const auto& constraint : constraints) {
        if (constraint->getPrimaryItemsAmount() > 0 && optionsSpan[constraintId].first.has_value()) {
          const auto& primaryItems = optionsSpan[constraintId].first.value();
          for (const auto& primaryItemId : primaryItems[globalOptionId]) {
            option.emplace_back(idOffsets[constraintId].first + primaryItemId);
          }
        }
        constraintId++;
      }
      constraintId = 0;
      for (const auto& constraint : constraints) {
        if (constraint->getSecondaryItemsAmount() > 0 && optionsSpan[constraintId].second.has_value()) {
          const auto& secondaryItems = optionsSpan[constraintId].second.value();
          for (const auto& secondaryItemId : secondaryItems[globalOptionId]) {
            option.emplace_back(idOffsets[constraintId].second + secondaryItemId);
          }
        }
        constraintId++;
      }
      options.emplace_back(option);
    }
    return DancingCellsStructure(primaryItemsCount, secondaryItemsCount, options);
  }

  Grid<puzzleSpace> solve() {
    auto solution = Grid<puzzleSpace>();

    auto structureCopy = structure;
    // Find a possible solution
    const auto solutionOptional = AlgorithmC::findOneSolution(structureCopy, seed);

    if (!solutionOptional.has_value()) {
      std::cout << "Cannot find a solution" << std::endl;
    }
    if (solutionOptional.has_value()) {
      const auto& options = solutionOptional.value();
      if (options.size() != puzzleSpace.columnsCount * puzzleSpace.rowsCount) {
        throw std::runtime_error(std::string("Solution found does not cover the entire grid."));
      }

      // Reduce structure's solution back to a grid:
      // Datastructure was created with available possibilities, options were given from first to last
      for (const auto [id, cell] : std::ranges::views::enumerate(possibilities)) {
        if (options.contains(id)) {
          const auto& [row, column, digit] = cell;
          solution[row][column] = digit;
        }
      }
    }

    return solution;
  }

private:
  /** Constructs the list of constraints
   * @param constraintTypes A bitflag of the constraints
   * @return The list of constructed constraints
   */
  std::vector<std::unique_ptr<ConstraintInterface<PuzzleIntrinsics<puzzleSpace>{}>>>
      createConstraints(const ConstraintType constraintTypes) const {
    std::vector<std::unique_ptr<ConstraintInterface<PuzzleIntrinsics<puzzleSpace>{}>>> constraintList;

    // CellConstraint constraint is always present
    constraintList.push_back(std::make_unique<CellConstraint<PuzzleIntrinsics<puzzleSpace>{}>>());

    for (uint8_t bitToCheck = 0; bitToCheck < 64; bitToCheck++) {
      const uint64_t valueToCheck = static_cast<uint64_t>(1) << bitToCheck;
      if (static_cast<uint64_t>(constraintTypes) & valueToCheck) {
        const ConstraintType singleConstraint = static_cast<ConstraintType>(valueToCheck);
        if (singleConstraint != ConstraintType::SUDOKU_CELL) {
          constraintList.push_back(
              ConstraintFactory::makeConstraint<PuzzleIntrinsics<puzzleSpace>{}>(singleConstraint));
        }
      }
    }

    return constraintList;
  }

  /** Selects only the valid clues out of the provided ones
   * @param clues A list of clues
   * @return The same list, but only with valid clues
   */
  std::unordered_set<Cell> getOnlyValidClues(const std::unordered_set<Cell>& clues) {
    std::unordered_set<Cell> result;
    for (const auto& cell : clues) {
      if (this->isValidCell(cell)) {
        result.insert(cell);
      }
    }
    return result;
  }

  /** Helper to compute the size of a specific option given the spans and its ID
   * @param optionSpan The option spans
   * @param optionId The ID of the option
   * @return The total size of the option at the given ID
   */
  static std::size_t computeOptionSize(const auto& optionSpan, std::size_t optionId) {
    std::size_t count = 0;
    for (const auto& span : optionSpan) {
      if (span.first.has_value()) {
        count += span.first.value()[optionId].size();
      }
      if (span.second.has_value()) {
        count += span.second.value()[optionId].size();
      }
    }
    return count;
  }

public:
  ///  The name of the puzzle
  const std::string name;

  /// The seed for the random number generator
  const std::optional<int32_t> seed;

  /// The list of constraint on the Puzzle
  const std::vector<std::unique_ptr<ConstraintInterface<PuzzleIntrinsics<puzzleSpace>{}>>> constraints;

  /// The cells with given values
  const std::unordered_set<Cell> givenCells;

  /// The list of available possiblities taking into account cells with given values
  const std::vector<Cell> possibilities;

  /// A 2D matrix of the grid, intialized with invalid digits
  const Grid<puzzleSpace> startingGrid = {};

  /// The data structure required for solving the puzzle
  const DancingCellsStructure structure;

  /// The solution to the puzzle
  const Grid<puzzleSpace> solution = {};
};