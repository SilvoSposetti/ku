#pragma once

#include "../Setter.hpp"
#include "../constraintTemplated/CellConstraint.hpp"
#include "../constraintTemplated/Constraint.hpp"
#include "../constraintTemplated/ConstraintInterface.hpp"
#include "../drawing/DataStructureDrawing.hpp"
#include "../drawing/DrawingOptions.hpp"
#include "../drawing/PuzzleDrawing.hpp"
#include "../solver/DancingCellsStructure.hpp"
#include "PuzzleIntrinsics.hpp"

#include <filesystem>
#include <ranges>
#include <span>
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
      , grid(initializeGrid())
      , structure(createStructure())
      , solution(solve()) {};

  std::array<std::array<Digit, puzzleSpace.columnsCount>, puzzleSpace.rowsCount> initializeGrid() const {
    auto grid =
        ArrayUtilities::create2DArray<Digit, puzzleSpace.columnsCount, puzzleSpace.rowsCount>(Digits::invalidDigit);
    for (const auto& cell : givenCells) {
      grid[cell.rowIndex][cell.columnIndex] = cell.digit;
    }
    return grid;
  }

  std::vector<Cell> constructActualPossibilities() const {
    const auto filter = [&](const Cell& possibility) {
      const auto found = std::ranges::find_if(
          this->givenCells, [&](const Cell& givenCell) { return possibility.isAtSameSpot(givenCell); });
      if (found == givenCells.end()) {
        return true;
      } else {
        const auto givenCell = *found;
        return possibility == givenCell;
      }
      return true;
    };
    auto filteredView = this->allPossibilities | std::ranges::views::filter(filter);
    return std::vector<Cell>(filteredView.begin(), filteredView.end());
  };

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
      std::ranges::transform(solution[rowIndex], std::back_inserter(digitStrings), [](const auto& digit) {
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

  /** Generates and stores the puzzle board to an SVG file in the provided directory
   * @param location Where the file should be stored
   * @return whether storing was successufl
   */
  bool exportDataStructureToSvg(const std::filesystem::path& location) const {

    const auto document = DataStructureDrawing::create<puzzleSpace>(name + "-ExactCover", structure, constraints, possibilities);
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

    std::vector<std::vector<XccElement>> options;
    int32_t globalOptionId = 0;
    options.reserve(possibilities.size());
    for (const auto& [i, j, possibleDigit] : this->allPossibilities) {
      const auto& actualDigit = grid[i][j];
      std::vector<XccElement> option;
      if (!Digits::isValid(actualDigit) || actualDigit == possibleDigit) {
        int32_t constraintId = 0;
        for (const auto& constraint : constraints) {
          const auto basePrimaryId = idOffsets[constraintId].first;
          if (constraint->getPrimaryItemsAmount() > 0) {
            const auto& primaryItems = constraint->getPrimaryItems();
            for (const auto& primaryItemId : primaryItems[globalOptionId]) {
              option.emplace_back(basePrimaryId + primaryItemId);
            }
          }
          constraintId++;
        }
        constraintId = 0;
        for (const auto& constraint : constraints) {
          if (constraint->getSecondaryItemsAmount() > 0) {
            const auto baseSecondaryId = idOffsets[constraintId].second;
            const auto& secondaryItems = constraint->getSecondaryItems();
            for (const auto& secondaryItemId : secondaryItems[globalOptionId]) {
              const auto itemId = secondaryItemId + baseSecondaryId;
              option.emplace_back(itemId);
            }
          }
          constraintId++;
        }
        options.emplace_back(option);
      }
      globalOptionId++;
    }
    return DancingCellsStructure(primaryItemsCount, secondaryItemsCount, options);
  }

  std::array<std::array<Digit, puzzleSpace.columnsCount>, puzzleSpace.rowsCount> solve() {

    // // TODO: remove this "clues" vector and pass only the given cells
    // auto clues = std::vector<std::vector<Sudo::Digit>>();
    // for (const auto& i : this->rowIndices) {
    //   auto row = std::vector<Sudo::Digit>();
    //   for (const auto& j : this->columnIndices) {
    //     row.push_back(static_cast<Sudo::Digit>(grid[i][j]));
    //   }
    //   clues.push_back(row);
    // }

    // const auto board = Setter::generate(clues, constraints, seed);

    // TODO: remove this "result" in a board and return solution directly
    auto result = std::array<std::array<Digit, puzzleSpace.columnsCount>, puzzleSpace.rowsCount>();
    // for (const auto& i : this->rowIndices) {
    //   auto& row = result[i];
    //   for (const auto& j : this->columnIndices) {
    //     row[j] = static_cast<Digit>(board->getSolution()[i][j]);
    //   }
    // }
    return result;
  }

private:
  /** Constructs the list of constraints
   * @param constraintTypes A bitflag of the constraints
   * @return The list of constructed constraints
   */
  std::vector<std::unique_ptr<ConstraintInterface<PuzzleIntrinsics<puzzleSpace>{}>>>
  createConstraints([[maybe_unused]] const ConstraintType constraintTypes) const {
    std::vector<std::unique_ptr<ConstraintInterface<PuzzleIntrinsics<puzzleSpace>{}>>> constraintList;

    constraintList.push_back(std::make_unique<CellConstraint<PuzzleIntrinsics<puzzleSpace>{}>>());

    // SUDOKU_CELL constraint is always present
    // constraintList.emplace_back(ConstraintFactory::makeConstraint(ConstraintType::SUDOKU_CELL));

    // for (int32_t bitToCheck = 0; bitToCheck < 64; bitToCheck++) {
    //   const uint64_t valueToCheck = static_cast<uint64_t>(1) << bitToCheck;
    //   if (static_cast<uint64_t>(constraintTypes) & valueToCheck) {
    //     const ConstraintType singleConstraint = static_cast<ConstraintType>(valueToCheck);
    //     if (singleConstraint != ConstraintType::SUDOKU_CELL) {
    //       constraintList.emplace_back(ConstraintFactory::makeConstraint(singleConstraint));
    //     }
    //   }
    // }

    return constraintList;
  }

  /** Selects only the valid clues out of the provided ones
   * @param clues A list of clues
   * @return The same list, but only with valid clues
   */
  std::unordered_set<Cell> getOnlyValidClues(const std::unordered_set<Cell>& clues) {
    std::unordered_set<Cell> result;
    for (const auto& cell : clues) {
      if (this->isCellValid(cell)) {
        result.insert(cell);
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
  const std::vector<std::unique_ptr<ConstraintInterface<PuzzleIntrinsics<puzzleSpace>{}>>> constraints;

  /// The cells with given values
  std::unordered_set<Cell> givenCells;

  const std::vector<Cell> possibilities;

  /// A 2D matrix of the grid, intialized with invalid digits
  std::array<std::array<Digit, puzzleSpace.columnsCount>, puzzleSpace.rowsCount> grid = this->emptyGrid;

  const DancingCellsStructure structure;

  std::array<std::array<Digit, puzzleSpace.columnsCount>, puzzleSpace.rowsCount> solution = this->emptyGrid;
};