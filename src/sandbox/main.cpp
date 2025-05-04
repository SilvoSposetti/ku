#include "puzzles/Puzzle.hpp"

#include <print>
#include <unordered_set>

int main() {
  const auto location = std::filesystem::path(OUT_DIR) / "sandbox";
  constexpr auto size = 4;
  constexpr auto puzzleSpace = PuzzleSpace{size, size, size};
  const auto clues = std::unordered_set<Cell>{
      {0, 0, 1},
      {1, 1, 2},
      {2, 2, 3},
  };
  const auto constraints = ConstraintType::NONE //
                           | ConstraintType::SUDOKU_ROW //
                           | ConstraintType::SUDOKU_COLUMN //
      ;
  const auto puzzle = Puzzle<puzzleSpace>("Name", clues, constraints, {});
  puzzle.exportDataStructureToSvg(location);
  puzzle.printGrid();
  puzzle.printSolution();

  return EXIT_SUCCESS;
}