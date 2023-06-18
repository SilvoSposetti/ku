#include "solver/AlgorithmX.h"

#include "doctest.h"
#include "solver/SparseCoordinateMatrix.h"

#include <algorithm>

TEST_CASE("Algorithm X") {
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
            {10, -1, 11},
            {-1, -1, -1}, // Empty option
            {-1, 12, 13},
            {14, 15, -1},
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
      REQUIRE(sparseMatrix.isSolvableByAlgorithmX()); // Theoretically solvable, but no solutions can be found
      //   AlgorithmX::printDataStructure(sparseMatrix);
      CHECK(!AlgorithmX::hasUniqueSolution(sparseMatrix));
      const std::vector<std::unordered_set<int32_t>> solutionRows = AlgorithmX::run(sparseMatrix);
      CHECK(solutionRows.empty());
    }
  }

  SUBCASE("Single solution, primary items only") {
    const std::vector<std::pair<std::vector<std::vector<int32_t>>, std::unordered_set<int32_t>>> exactCoverMatrices = {
        {
            // Example 1
            {
                {-1, -1, 10, -1, 11, -1, -1}, // Part of solution 1
                {12, -1, -1, 13, -1, -1, 14},
                {-1, 15, 16, -1, -1, 17, -1},
                {18, -1, -1, 19, -1, 20, -1}, // Part of solution 1
                {-1, 21, -1, -1, -1, -1, 22}, // Part of solution 1
                {-1, -1, -1, 23, 24, -1, 25},
            },
            {0, 3, 4}, // Solution 1
        },
        {
            // Example 2
            {
                {10, -1, -1, 11, -1, -1, 12},
                {13, -1, -1, 14, -1, -1, -1}, // Part of solution 2
                {-1, -1, -1, 15, 16, -1, 17},
                {-1, -1, 18, -1, 19, 20, -1}, // Part of solution 2
                {-1, 21, 22, -1, -1, 23, 24},
                {-1, 25, -1, -1, -1, -1, 30}, // Part of solution 2
                {-1, -1, -1, -1, -1, -1, -1}, // Empty option

            },
            {1, 3, 5}, // Solution 2
        },
        {
            // Example 3
            {
                {10, 11, -1, -1, -1, -1}, // Part of solution 3
                {-1, -1, -1, -1, 12, 13}, // Part of solution 3
                {-1, -1, -1, 14, 15, -1},
                {16, -1, 18, -1, -1, -1},
                {-1, -1, -1, -1, -1, -1}, // Empty option
                {-1, -1, 21, -1, -1, -1}, // Part of solution 3
                {-1, -1, -1, 22, -1, -1}, // Part of solution 3
                {23, -1, 24, -1, -1, 26},
            },
            {0, 1, 5, 6}, // Solution 3
        },
        {
            // Example 4
            {
                {-1, -1, -1, -1, -1, -1}, // Empty option
                {10, 11, 12, 13, 14, 15}, // Single option containing all items
            },
            {1}, // Solution 4
        },
        {
            // Example 5
            {
                {10},
            },
            {0}, // Solution 5
        },
        {
            // Example 6
            {
                {10, 11},
            },
            {0} // Solution 6
        },
    };

    for (const auto& [exactCoverMatrix, exactCoverSolution] : exactCoverMatrices) {
      SparseCoordinateMatrix sparseMatrix(exactCoverMatrix);
      REQUIRE(sparseMatrix.isSolvableByAlgorithmX());
      //   AlgorithmX::printDataStructure(sparseMatrix);
      CHECK(AlgorithmX::hasUniqueSolution(sparseMatrix));
      const std::vector<std::unordered_set<int32_t>> solutions = AlgorithmX::run(sparseMatrix);
      CHECK(solutions.size() == 1);
      CHECK(solutions.at(0) == exactCoverSolution);
    }
  }

  SUBCASE("Single solution, primary and secondary items") {
    const std::vector<std::tuple<std::vector<std::vector<int32_t>>, std::vector<int32_t>, std::unordered_set<int32_t>>>
        exactCoverMatrices = {
            {
                // Example 1
                {
                    {10, -1, -1, -1, -1, -1, -1}, // Part of solution 1
                    {-1, 11, -1, -1, 12, -1, -1}, // Part of solution 1
                    {-1, 13, -1, -1, -1, 14, -1},
                    {-1, -1, 15, -1, 16, 17, -1},
                    {-1, -1, 18, -1, -1, 19, 20},
                    {-1, -1, 21, -1, -1, 22, -1}, // Part of solution 1
                    {-1, -1, -1, 23, -1, -1, 24}, // Part of solution 1
                },
                {4, 5, 6}, // Indices of the items that are secondary
                {0, 1, 5, 6}, // Solution 1
            },
            {
                // Example 2
                {
                    {-1, 10, -1, 11},
                    {-1, 12, -1, -1}, // Part of solution 2
                    {13, -1, -1, -1}, // Part of solution 2
                    {-1, -1, -1, -1},
                    {-1, -1, 15, 16}, // Part of solution 2
                    {17, -1, -1, 18},
                },
                {3}, // Indices of the items that are secondary
                {1, 2, 4}, // Solution 2
            },
            {
                // Example 3 (like Example 2, but with the secondary colum moved)
                {
                    {11, -1, 10, -1},
                    {-1, -1, 12, -1}, // Part of solution 3
                    {-1, 13, -1, -1}, // Part of solution 3
                    {-1, -1, -1, -1},
                    {16, -1, -1, 15}, // Part of solution 3
                    {18, 17, -1, -1},
                },
                {0}, // Indices of the items that are secondary
                {1, 2, 4}, // Solution 2
            },
            {
                // Example 4 (empty secondary items)
                {
                    {13, -1, -1, -1, -1, -1}, // Part of solution 4
                    {-1, -1, 14, -1, -1, -1},
                    {-1, -1, -1, 12, -1, -1}, // Part of solution 4
                    {-1, -1, 16, 17, -1, -1},
                    {-1, 10, 11, -1, -1, -1}, // Part of solution 4
                },
                {4, 5}, // Indices of the items that are secondary
                {0, 2, 4}, // Solution 4
            },
            {
                // Example 5
                {
                    {-1, -1, -1, 10, -1}, //  No primary items! This solution is not selected
                    {11, -1, 12, -1, -1}, //  Part of solution 5
                    {-1, -1, -1, -1, -1},
                    {-1, 13, -1, -1, 14}, //  Part of solution 5
                },
                {2, 3, 4}, // Indices of the items that are secondary
                {1, 3}, // Solution 5
            },
        };

    for (const auto& [exactCoverMatrix, secondaryItemIndices, exactCoverSolution] : exactCoverMatrices) {
      SparseCoordinateMatrix sparseMatrix(exactCoverMatrix);
      for (const auto& secondaryItemIndex : secondaryItemIndices) {
        sparseMatrix.setColumnSecondary(secondaryItemIndex);
        CHECK(!sparseMatrix.isColumnPrimary(secondaryItemIndex));
      }
      REQUIRE(sparseMatrix.isSolvableByAlgorithmX());
      //   AlgorithmX::printDataStructure(sparseMatrix);
      CHECK(AlgorithmX::hasUniqueSolution(sparseMatrix));
      const std::vector<std::unordered_set<int32_t>> solutions = AlgorithmX::run(sparseMatrix);
      CHECK(solutions.size() == 1);
      CHECK(solutions.at(0) == exactCoverSolution);
    }
  }

  SUBCASE("Multiple solutions, primary items only") {
    const std::vector<std::pair<std::vector<std::vector<int32_t>>, std::vector<std::unordered_set<int32_t>>>>
        exactCoverMatrices = {
            {
                // Example 1
                {
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
                // Example 2
                {
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
                // Example 3
                {
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
            {
                // Example 4
                {
                    {10, -1}, // Solution 4.A | 4.B | 4.C
                    {11, -1}, // Solution 4.D | 4.E | 4.F
                    {12, -1}, // Solution 4.G | 4.H | 4.I
                    {-1, 13}, // Solution 4.A | 4.D | 4.G
                    {-1, 14}, // Solution 4.B | 4.E | 4.H
                    {-1, 15}, // Solution 4.C | 4.F | 4.I
                },
                {
                    {0, 3}, // Solution 4.A
                    {0, 4}, // Solution 4.B
                    {0, 5}, // Solution 4.C
                    {1, 3}, // Solution 4.D
                    {1, 4}, // Solution 4.E
                    {1, 5}, // Solution 4.F
                    {2, 3}, // Solution 4.G
                    {2, 4}, // Solution 4.H
                    {2, 5}, // Solution 4.I
                },
            },
            {
                // Example 5
                {
                    {10}, // Solution 5.A
                    {11}, // Solution 5.B
                    {12}, // Solution 5.C
                },
                {
                    {0}, // Solution 5.A
                    {1}, // Solution 5.B
                    {2}, // Solution 5.C
                },
            },
        };

    for (const auto& [exactCoverMatrix, exactCoverSolutions] : exactCoverMatrices) {
      SparseCoordinateMatrix sparseMatrix(exactCoverMatrix);
      REQUIRE(sparseMatrix.isSolvableByAlgorithmX());
      //   AlgorithmX::printDataStructure(sparseMatrix);
      const std::vector<std::unordered_set<int32_t>> solutions = AlgorithmX::run(sparseMatrix);
      // Check that every solution found appears in the list of the ones provided
      CHECK(solutions.size() == exactCoverSolutions.size());
      for (const auto& exactCoverSolution : exactCoverSolutions) {
        CHECK(std::count(solutions.begin(), solutions.end(), exactCoverSolution) == 1);
      }
    }
  }

  SUBCASE("Multiple solutions, primary and secondary items") {
    const std::vector<
        std::tuple<std::vector<std::vector<int32_t>>, std::vector<int32_t>, std::vector<std::unordered_set<int32_t>>>>
        exactCoverMatrices = {
            {
                // Example 1
                {
                    {-1, -1, 10, -1, 11, -1, -1}, // Part of solution 1.A
                    {12, -1, -1, 13, -1, -1, 14}, // Part of solution 1.B
                    {-1, 15, 16, -1, -1, 17, -1}, // Part of solution 1.B
                    {18, -1, -1, 19, -1, 20, -1}, // Part of solution 1.A
                    {-1, 21, -1, -1, -1, -1, 22}, // Part of solution 1.A
                    {-1, -1, -1, 23, 24, -1, 25},
                },
                {4, 5, 6}, // Indices of the items that are secondary
                {
                    {0, 3, 4}, // Solution 1.A
                    {1, 2}, // Solution 1.B
                },
            },
            {
                // Example 2
                {
                    {-1, -1, 10, -1}, // Part of solution 2.A
                    {12, -1, -1, 13}, // Part of solution 2.B
                    {-1, 15, 16, -1}, // Part of solution 2.B
                    {-1, -1, -1, -1},
                    {14, 15, -1, -1}, // Part of solution 2.A
                },
                {3}, // Indices of the items that are secondary
                {
                    {0, 4}, // Solution 2.A
                    {1, 2}, // Solution 2.B
                },
            },
            {
                // Example 3 (like Example 2, but with the secondary colum moved)
                {
                    {-1, -1, -1, 10}, // Part of solution 3.A
                    {12, 13, -1, -1}, // Part of solution 3.B
                    {-1, -1, 15, 16}, // Part of solution 3.B
                    {-1, -1, -1, -1},
                    {14, -1, 15, -1}, // Part of solution 3.A
                },
                {1}, // Indices of the items that are secondary
                {
                    {0, 4}, // Solution 3.A
                    {1, 2}, // Solution 3.B
                },
            },
            {
                // Example 4
                {
                    {-1, 10, -1, -1, -1}, //  Part of solution 4.A
                    {11, -1, 12, -1, 13}, //  Part of solution 4.A | 4.B | 4.C
                    {-1, -1, -1, -1, -1},
                    {-1, 14, -1, 15, 16},
                    {-1, 17, -1, 18, -1}, //  Part of solution 4.B
                    {-1, 19, -1, -1, -1}, //  Part of solution 4.C
                },
                {3, 4}, // Indices of the items that are secondary
                {
                    {0, 1}, // Solution 4.A
                    {1, 4}, // Solution 4.B
                    {1, 5}, // Solution 4.C
                },
            },
        };

    for (const auto& [exactCoverMatrix, secondaryItemIndices, exactCoverSolutions] : exactCoverMatrices) {
      SparseCoordinateMatrix sparseMatrix(exactCoverMatrix);
      for (const auto& secondaryItemIndex : secondaryItemIndices) {
        sparseMatrix.setColumnSecondary(secondaryItemIndex);
        CHECK(!sparseMatrix.isColumnPrimary(secondaryItemIndex));
      }
      REQUIRE(sparseMatrix.isSolvableByAlgorithmX());
      //   AlgorithmX::printDataStructure(sparseMatrix);
      const std::vector<std::unordered_set<int32_t>> solutions = AlgorithmX::run(sparseMatrix);
      // Check that every solution found appears in the list of the ones provided
      CHECK(solutions.size() == exactCoverSolutions.size());
      for (const auto& exactCoverSolution : exactCoverSolutions) {
        CHECK(std::count(solutions.begin(), solutions.end(), exactCoverSolution) == 1);
      }
    }
  }
}
