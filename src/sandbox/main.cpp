#include "Sudoku.hpp"
#include "puzzles/Puzzle.hpp"
#include "utilities/Timer.hpp"

#include <print>
#include <unordered_set>

int main() {
  const auto location = std::filesystem::path(OUT_DIR) / "sandbox";
  constexpr auto size = 9;
  constexpr auto puzzleSpace = PuzzleSpace{size, size, size};
  const auto clues = std::unordered_set<Cell>{
      {0, 0, 1},
      {1, 1, 2},
      {2, 2, 3},
  };
  const auto constraints = ConstraintType::NONE //
                           | ConstraintType::EXACT_ROW //
                           | ConstraintType::EXACT_COLUMN //
                           | ConstraintType::EXACT_3x3_BOXES //
                           | ConstraintType::EXACT_POSITIVE_DIAGONAL //
                           | ConstraintType::EXACT_NEGATIVE_DIAGONAL //
      ;

  Timer timer;
  std::size_t count = 1;
  for (std::size_t i = 0; i < count; i++) {
    const auto puzzle = Puzzle<puzzleSpace>("Name", clues, constraints, {});
    puzzle.exportDataStructureToSvg(location);
    // puzzle.printGrid();
    puzzle.printSolution();
    // auto sudoku = Sudoku("OldSudoku", constraints, 81, i);
    // sudoku.exportExactCoverMatrixToSvg(location);
  }
  timer.printElapsed("Total time for " + std::to_string(count) + " " + (count > 1 ? "puzzles" : "puzzle") + ":");
  return EXIT_SUCCESS;
}