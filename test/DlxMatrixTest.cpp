#include "solver/DlxMatrix.h"

#include "doctest.h"
#include "solver/SparseCoordinateMatrix.h"

#include <algorithm>

TEST_CASE("DLX Matrix") {
  SUBCASE("No solutions") {
    const std::vector<std::vector<std::vector<int32_t>>> unsolvableMatrices = {
        {
            // Empty matrix
        },
        {
            // Invalid (fully-empty) matrix
            {-1, -1, -1, -1, -1},
            {-1, -1, -1, -1, -1},
            {-1, -1, -1, -1, -1},
        },
        {
            // Valid, but no solution possible
            {1, -1, 1},
            {-1, -1, -1}, // Empty option
            {-1, 1, 1},
            {1, 1, -1},
            {-1, -1, -1}, // Empty option
        },
        {
            // Valid, but no solution possible
            {-1, -1, 10, -1, 11, -1, -1},
            {12, -1, -1, 13, -1, -1, 14},
            {-1, 15, 16, -1, -1, 17, -1},
            {18, -1, -1, 19, -1, -1, -1},
            {-1, 21, -1, -1, -1, -1, 22},
            {-1, -1, -1, 23, 24, -1, 25},
        },
        {
            // Valid, but no solution possible
            {10, 11, -1, 12, 13, 14, 15},
            {-1, 16, 17, -1, -1, -1, -1},
            {-1, -1, 18, 19, -1, -1, -1},
            {-1, -1, -1, -1, 20, 21, -1},
            {-1, -1, -1, 22, -1, -1, -1},
            {-1, -1, -1, -1, -1, -1, -1}, // Empty option
            {23, 24, 25, -1, -1, -1, -1},
        },
        {
            // Valid, but no solution possible
            {-1, 10, 11},
            {12, 13, -1},
        },
    };
    for (const auto& unsolvableMatrix : unsolvableMatrices) {
      SparseCoordinateMatrix sparseMatrix(unsolvableMatrix);
      CHECK(sparseMatrix.isSolvableByDlx()); // Theoretically solvable, but no solutions can be found
      //   AlgorithmX::printDataStructure(sparseMatrix);
      CHECK(!AlgorithmX::hasUniqueSolution(sparseMatrix));
      const std::vector<std::unordered_set<int32_t>> solutionRows = AlgorithmX::run(sparseMatrix);
      CHECK(solutionRows.empty());
    }
  }

  SUBCASE("Single solution, primary items only") {
    const std::vector<std::pair<std::vector<std::vector<int32_t>>, std::unordered_set<int32_t>>> exactCoverMatrices = {
        {{
             // Example 1
             {-1, -1, 10, -1, 11, -1, -1}, // Part of solution 1
             {12, -1, -1, 13, -1, -1, 14},
             {-1, 15, 16, -1, -1, 17, -1},
             {18, -1, -1, 19, -1, 20, -1}, // Part of solution 1
             {-1, 21, -1, -1, -1, -1, 22}, // Part of solution 1
             {-1, -1, -1, 23, 24, -1, 25},
         },
         {0, 3, 4}}, // Solution 1
        {{
             // Example 2
             {10, -1, -1, 11, -1, -1, 12},
             {13, -1, -1, 14, -1, -1, -1}, // Part of solution 2
             {-1, -1, -1, 15, 16, -1, 17},
             {-1, -1, 18, -1, 19, 20, -1}, // Part of solution 2
             {-1, 21, 22, -1, -1, 23, 24},
             {-1, 25, -1, -1, -1, -1, 30}, // Part of solution 2
             {-1, -1, -1, -1, -1, -1, -1}, // Empty option

         },
         {1, 3, 5}}, // Solution 2
        {{
             // Example 3
             {10, 11, -1, -1, -1, -1}, // Part of solution 3
             {-1, -1, -1, -1, 12, 13}, // Part of solution 3
             {-1, -1, -1, 14, 15, -1},
             {16, -1, 18, -1, -1, -1},
             {-1, -1, -1, -1, -1, -1}, // Empty option
             {-1, -1, 21, -1, -1, -1}, // Part of solution 3
             {-1, -1, -1, 22, -1, -1}, // Part of solution 3
             {23, -1, 24, -1, -1, 26},
         },
         {0, 1, 5, 6}}, // Solution 3
        {{
             // Example 4
             {-1, -1, -1, -1, -1, -1}, // Empty option
             {10, 11, 12, 13, 14, 15}, // Single option containing all items
         },
         {1}}, // Solution 4
    };

    for (const auto& [exactCoverMatrix, exactCoverSolution] : exactCoverMatrices) {
      SparseCoordinateMatrix sparseMatrix(exactCoverMatrix);
      CHECK(sparseMatrix.isSolvableByDlx());
      //   AlgorithmX::printDataStructure(sparseMatrix);
      CHECK(AlgorithmX::hasUniqueSolution(sparseMatrix));
      const std::vector<std::unordered_set<int32_t>> solutions = AlgorithmX::run(sparseMatrix);
      CHECK(solutions.size() == 1);
      CHECK(solutions.at(0) == exactCoverSolution);
    }
  }

  SUBCASE("Single solution, primary and secondary items") {}
  SUBCASE("Multiple solutions, primary items only") {
    const std::vector<std::pair<std::vector<std::vector<int32_t>>, std::vector<std::unordered_set<int32_t>>>>
        exactCoverMatrices = {
            {
                {
                    // Example 1
                    {12, -1, -1, 13, -1, -1, 14},
                    {18, -1, -1, 19, -1, 20, -1}, // Part of solution 1.A
                    {-1, 28, -1, -1, 29, 30, -1}, // Part of solution 1.B
                    {-1, 15, 16, -1, -1, 17, -1},
                    {-1, -1, 26, 27, -1, -1, -1}, // Part of solution 1.B
                    {-1, -1, 10, -1, 11, -1, -1}, // Part of solution 1.A
                    {-1, 21, -1, -1, -1, -1, 22}, // Part of solution 1.A
                    {-1, -1, -1, 23, 24, -1, 25},
                    {-1, -1, -1, -1, -1, -1, -1}, // Empty option
                    {33, -1, -1, -1, -1, -1, 34}, // Part of solution 1.B
                    {-1, -1, 31, -1, -1, 32, -1},
                },
                {
                    {1, 5, 6}, // Solution 1.A
                    {2, 4, 9}, // Solution 1.B
                },
            },
            {
                {
                    // Example 2
                    {10, -1, -1, -1}, // Part of solution 2.A | 2.B
                    {-1, 11, -1, -1}, // Part of solution 2.A
                    {-1, -1, 12, -1}, // Part of solution 2.A | 2.C
                    {-1, -1, -1, 13}, // Part of solution 2.A | 2.B | 2.C
                    {-1, 14, 15, -1}, // Part of solution 2.B
                    {16, 17, -1, -1}, // Part of solution 2.C
                },
                {
                    {0, 1, 2, 3}, // Solution 2.A
                    {0, 3, 4}, // Solution 2.B
                    {2, 3, 5}, // Solution 2.C
                },
            },
            {
                {
                    // Example 3
                    {-1, -1, -1}, // Empty option
                    {13, -1, -1}, // Solution 3.A | 3.B
                    {-1, 14, -1}, // Solution 3.A | 3.C
                    {-1, -1, 15}, // Solution 3.A | 3.D
                    {16, 17, -1}, // Solution 3.D
                    {18, -1, 19}, // Solution 3.C
                    {-1, 20, 21}, // Solution 3.B
                    {10, 11, 12}, // Solution 3.E
                },
                {
                    {1, 2, 3}, // Solution 3.A
                    {1, 6}, // Solution 3.B
                    {4, 3}, // Solution 3.C
                    {5, 2}, // Solution 3.D
                    {7}, // Solution 3.E
                },
            },
        };

    for (const auto& [exactCoverMatrix, exactCoverSolutions] : exactCoverMatrices) {
      SparseCoordinateMatrix sparseMatrix(exactCoverMatrix);
      CHECK(sparseMatrix.isSolvableByDlx());
      //   AlgorithmX::printDataStructure(sparseMatrix);
      const std::vector<std::unordered_set<int32_t>> solutions = AlgorithmX::run(sparseMatrix);
      CHECK(solutions.size() == exactCoverSolutions.size());
      // Check that every solution found appears in the list of the ones provided
      for (const auto& exactCoverSolution : exactCoverSolutions) {
        CHECK(std::count(solutions.begin(), solutions.end(), exactCoverSolution) == 1);
      }
    }
  }

  SUBCASE("Multiple solutions, primary and secondary items") {}
}
