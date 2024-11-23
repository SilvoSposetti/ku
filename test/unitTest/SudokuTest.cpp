#include "Sudoku.hpp"

#include "Sudo.hpp"

#include <doctest.h>

TEST_CASE("Sudoku") {
  ConstraintType classicConstraints = ConstraintType::SUDOKU_CELL | ConstraintType::SUDOKU_ROW |
                                      ConstraintType::SUDOKU_COLUMN | ConstraintType::SUDOKU_BOX;

  const auto checkSolution = [](const std::vector<std::vector<int32_t>>& computed,
                                const std::vector<std::vector<Sudo::Digit>>& expected) {
    const int32_t rowSize = expected.size();
    const int32_t columnSize = expected[0].size();

    for (int32_t i = 0; i < rowSize; i++) {
      for (int32_t j = 0; j < columnSize; j++) {
        if (static_cast<int32_t>(expected[i][j]) != computed[i][j]) {
          return false;
        }
      }
    }
    return true;
  };

  SUBCASE("Large search tree") {
    const std::vector<std::vector<int32_t>> input = {
        {1, 2, 0, 3, 0, 0, 4, 0, 0},
        {5, 0, 0, 4, 0, 0, 1, 0, 0},
        {0, 0, 0, 0, 2, 0, 0, 6, 0},
        {7, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 7, 0, 0, 0, 3, 1},
        {0, 0, 0, 0, 5, 4, 7, 0, 0},
        {4, 0, 0, 5, 0, 0, 3, 0, 0},
        {0, 8, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 9, 0, 4, 0, 0, 0, 0},
    };

    const std::vector<std::vector<int32_t>> solution = {
        {1, 2, 8, 3, 6, 5, 4, 7, 9},
        {5, 9, 6, 4, 7, 8, 1, 2, 3},
        {3, 4, 7, 9, 2, 1, 8, 6, 5},
        {7, 1, 5, 6, 3, 9, 2, 4, 8},
        {9, 6, 4, 7, 8, 2, 5, 3, 1},
        {8, 3, 2, 1, 5, 4, 7, 9, 6},
        {4, 7, 1, 5, 9, 6, 3, 8, 2},
        {6, 8, 3, 2, 1, 7, 9, 5, 4},
        {2, 5, 9, 8, 4, 3, 6, 1, 7},
    };

    Sudoku sudoku("LargeSearchTree", input, classicConstraints);
    CHECK(checkSolution(solution, sudoku.getSolution()));
  }

  SUBCASE("Digits of pi with 32 clues (1)") {
    const std::vector<std::vector<int32_t>> input = {
        {0, 0, 3, 0, 1, 0, 0, 0, 0},
        {4, 1, 5, 0, 0, 0, 0, 9, 0},
        {2, 0, 6, 5, 0, 0, 3, 0, 0},
        {5, 0, 0, 0, 8, 0, 0, 0, 9},
        {0, 7, 0, 9, 0, 0, 0, 3, 2},
        {0, 3, 8, 0, 0, 4, 0, 6, 0},
        {0, 0, 0, 2, 6, 0, 4, 0, 3},
        {0, 0, 0, 3, 0, 0, 0, 0, 8},
        {3, 2, 0, 0, 0, 7, 9, 5, 0},
    };

    const std::vector<std::vector<int32_t>> solution = {
        {7, 9, 3, 4, 1, 2, 6, 8, 5},
        {4, 1, 5, 6, 3, 8, 2, 9, 7},
        {2, 8, 6, 5, 7, 9, 3, 1, 4},
        {5, 6, 2, 1, 8, 3, 7, 4, 9},
        {1, 7, 4, 9, 5, 6, 8, 3, 2},
        {9, 3, 8, 7, 2, 4, 5, 6, 1},
        {8, 5, 9, 2, 6, 1, 4, 7, 3},
        {6, 4, 7, 3, 9, 5, 1, 2, 8},
        {3, 2, 1, 8, 4, 7, 9, 5, 6},
    };

    Sudoku sudoku("DigitsOfPi32Clues2", input, classicConstraints);
    CHECK(checkSolution(solution, sudoku.getSolution()));
  }

  SUBCASE("Digits of pi with 32 clues (2)") {
    // Note: The 33rd digit of pi is a 0
    // 17 clues is the minimum allowed for a classic sudoku to may still have a unique solution
    const std::vector<std::vector<int32_t>> input = {
        {0, 0, 0, 0, 0, 3, 0, 1, 4},
        {0, 0, 0, 0, 0, 1, 5, 9, 2},
        {0, 0, 0, 0, 6, 5, 0, 0, 3},
        {0, 0, 0, 0, 5, 8, 9, 0, 7},
        {0, 0, 9, 0, 0, 0, 3, 2, 0},
        {0, 3, 8, 4, 0, 0, 0, 0, 6},
        {2, 6, 0, 0, 0, 4, 0, 3, 0},
        {0, 0, 0, 0, 3, 0, 0, 8, 0},
        {0, 0, 3, 2, 7, 9, 0, 0, 5},
    };

    const std::vector<std::vector<int32_t>> solution = {
        {5, 9, 7, 8, 2, 3, 6, 1, 4},
        {3, 8, 6, 7, 4, 1, 5, 9, 2},
        {1, 4, 2, 9, 6, 5, 8, 7, 3},
        {6, 2, 1, 3, 5, 8, 9, 4, 7},
        {4, 5, 9, 6, 1, 7, 3, 2, 8},
        {7, 3, 8, 4, 9, 2, 1, 5, 6},
        {2, 6, 5, 1, 8, 4, 7, 3, 9},
        {9, 7, 4, 5, 3, 6, 2, 8, 1},
        {8, 1, 3, 2, 7, 9, 4, 6, 5},
    };

    Sudoku sudoku("DigitsOfPiWith32Clues2", input, classicConstraints);
    CHECK(checkSolution(solution, sudoku.getSolution()));
  }

  SUBCASE("Digits of pi with 17 clues") {
    // 17 clues is the minimum allowed for a classic sudoku to may still have a unique solution
    const std::vector<std::vector<int32_t>> input = {
        {0, 0, 0, 0, 0, 0, 3, 0, 0},
        {1, 0, 0, 4, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0, 5},
        {9, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 2, 6, 0, 0},
        {0, 0, 0, 0, 5, 3, 0, 0, 0},
        {0, 5, 0, 8, 0, 0, 0, 0, 0},
        {0, 0, 0, 9, 0, 0, 0, 7, 0},
        {0, 8, 3, 0, 0, 0, 0, 4, 0},
    };

    const std::vector<std::vector<int32_t>> solution = {
        {5, 9, 7, 2, 1, 8, 3, 6, 4},
        {1, 3, 2, 4, 6, 5, 8, 9, 7},
        {8, 6, 4, 3, 7, 9, 1, 2, 5},
        {9, 1, 5, 6, 8, 4, 7, 3, 2},
        {3, 4, 8, 7, 9, 2, 6, 5, 1},
        {2, 7, 6, 1, 5, 3, 4, 8, 9},
        {6, 5, 9, 8, 4, 7, 2, 1, 3},
        {4, 2, 1, 9, 3, 6, 5, 7, 8},
        {7, 8, 3, 5, 2, 1, 9, 4, 6},
    };

    Sudoku sudoku("DigitsOfPi17Clues", input, classicConstraints);
    CHECK(checkSolution(solution, sudoku.getSolution()));
  }

  SUBCASE("SolvedWithOnlyNakedSingles") {
    // 17 clues is the minimum allowed for a classic sudoku to may still have a unique solution
    const std::vector<std::vector<int32_t>> input = {
        {0, 0, 3, 0, 1, 0, 0, 0, 0},
        {4, 1, 5, 0, 0, 0, 0, 9, 0},
        {2, 0, 6, 5, 0, 0, 3, 0, 0},
        {5, 0, 0, 0, 8, 0, 0, 0, 9},
        {0, 7, 0, 9, 0, 0, 0, 3, 2},
        {0, 3, 8, 0, 0, 4, 0, 6, 0},
        {0, 0, 0, 2, 6, 0, 4, 0, 3},
        {0, 0, 0, 3, 0, 0, 0, 0, 8},
        {3, 2, 0, 0, 0, 7, 9, 5, 0},
    };

    const std::vector<std::vector<int32_t>> solution = {
        {7, 9, 3, 4, 1, 2, 6, 8, 5},
        {4, 1, 5, 6, 3, 8, 2, 9, 7},
        {2, 8, 6, 5, 7, 9, 3, 1, 4},
        {5, 6, 2, 1, 8, 3, 7, 4, 9},
        {1, 7, 4, 9, 5, 6, 8, 3, 2},
        {9, 3, 8, 7, 2, 4, 5, 6, 1},
        {8, 5, 9, 2, 6, 1, 4, 7, 3},
        {6, 4, 7, 3, 9, 5, 1, 2, 8},
        {3, 2, 1, 8, 4, 7, 9, 5, 6},
    };

    Sudoku sudoku("SolvedWithOnlyNakedSingles", input, classicConstraints);
    CHECK(checkSolution(solution, sudoku.getSolution()));
  }
}

// Digits of pi, 16 clues: There's two solutions
// std::vector<std::vector<int32_t>> digits = {
//     {0, 3, 0, 0, 1, 0, 0, 0, 0},
//     {0, 0, 0, 4, 0, 0, 1, 0, 0},
//     {0, 5, 0, 0, 0, 0, 0, 9, 0},
//     {2, 0, 0, 0, 0, 0, 6, 0, 4},
//     {0, 0, 0, 0, 3, 5, 0, 0, 0},
//     {1, 0, 0, 0, 0, 0, 0, 0, 0},
//     {4, 0, 0, 6, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 5, 0},
//     {0, 9, 0, 0, 0, 0, 0, 0, 0},
// };
