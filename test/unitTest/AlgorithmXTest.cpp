#include "solver/AlgorithmX.h"

#include "doctest.h"
#include "solver/SparseCoordinateMatrix.h"

#include <algorithm>

TEST_CASE("Algorithm X") {

  const std::vector<std::optional<int32_t>> seeds = {std::nullopt, 0, 1, -566, 9845};

  SUBCASE("No solutions") {
    const std::vector<std::vector<std::vector<bool>>> unsolvableMatrices = {
        {
            // Empty matrix
        },
        {
            // Valid, but no solution possible
            {1, 0, 1},
            {0, 0, 0}, // Empty option
            {0, 1, 1},
            {1, 1, 0},
            {0, 0, 0}, // Empty option
        },
        {
            // Valid, but no solution possible
            {0, 0, 1, 0, 1, 0, 0},
            {1, 0, 0, 1, 0, 0, 1},
            {0, 1, 1, 0, 0, 1, 0},
            {1, 0, 0, 1, 0, 0, 0},
            {0, 1, 0, 0, 0, 0, 1},
            {0, 0, 0, 1, 1, 0, 1},
        },
        {
            // Valid, but no solution possible
            {1, 1, 0, 1, 1, 1, 1},
            {0, 1, 1, 0, 0, 0, 0},
            {0, 0, 1, 1, 0, 0, 0},
            {0, 0, 0, 0, 1, 1, 0},
            {0, 0, 0, 1, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0}, // Empty option
            {1, 1, 1, 0, 0, 0, 0},
        },
        {
            // Valid, but no solution possible
            {0, 1, 1},
            {1, 1, 0},
        },
    };
    for (const auto& seed : seeds) {
      for (const auto& unsolvableMatrix : unsolvableMatrices) {
        SparseCoordinateMatrix sparseMatrix(unsolvableMatrix);
        // Theoretically solvable, but no solutions can be found
        REQUIRE(sparseMatrix.isSolvableByAlgorithmX());
        //   AlgorithmX::printDataStructure(sparseMatrix);
        CHECK(!AlgorithmX::hasUniqueSolution(sparseMatrix, seed));
        CHECK(AlgorithmX::findAllSolutions(sparseMatrix, seed).empty());
        CHECK(AlgorithmX::findOneSolution(sparseMatrix, seed).empty());
      }
    }
  }

  SUBCASE("Single solution, primary items only") {
    const std::vector<std::pair<std::vector<std::vector<bool>>, std::unordered_set<int32_t>>> exactCoverMatrices = {
        {
            // Example 1
            {
                {0, 0, 1, 0, 1, 0, 0}, // Part of solution 1
                {1, 0, 0, 1, 0, 0, 1},
                {0, 1, 1, 0, 0, 1, 0},
                {1, 0, 0, 1, 0, 1, 0}, // Part of solution 1
                {0, 1, 0, 0, 0, 0, 1}, // Part of solution 1
                {0, 0, 0, 1, 1, 0, 1},
            },
            {0, 3, 4}, // Solution 1
        },
        {
            // Example 2
            {
                {1, 0, 0, 1, 0, 0, 1},
                {1, 0, 0, 1, 0, 0, 0}, // Part of solution 2
                {0, 0, 0, 1, 1, 0, 1},
                {0, 0, 1, 0, 1, 1, 0}, // Part of solution 2
                {0, 1, 1, 0, 0, 1, 1},
                {0, 1, 0, 0, 0, 0, 1}, // Part of solution 2
                {0, 0, 0, 0, 0, 0, 0}, // Empty option

            },
            {1, 3, 5}, // Solution 2
        },
        {
            // Example 3
            {
                {1, 1, 0, 0, 0, 0}, // Part of solution 3
                {0, 0, 0, 0, 1, 1}, // Part of solution 3
                {0, 0, 0, 1, 1, 0},
                {1, 0, 1, 0, 0, 0},
                {0, 0, 0, 0, 0, 0}, // Empty option
                {0, 0, 1, 0, 0, 0}, // Part of solution 3
                {0, 0, 0, 1, 0, 0}, // Part of solution 3
                {1, 0, 1, 0, 0, 1},
            },
            {0, 1, 5, 6}, // Solution 3
        },
        {
            // Example 4
            {
                {0, 0, 0, 0, 0, 0}, // Empty option
                {1, 1, 1, 1, 1, 1}, // Single option containing all items
            },
            {1}, // Solution 4
        },
        {
            // Example 5
            {
                {1},
            },
            {0}, // Solution 5
        },
        {
            // Example 6
            {
                {1, 1},
            },
            {0} // Solution 6
        },
    };
    for (const auto& seed : seeds) {
      for (const auto& [exactCoverMatrix, exactCoverSolution] : exactCoverMatrices) {
        SparseCoordinateMatrix sparseMatrix(exactCoverMatrix);
        REQUIRE(sparseMatrix.isSolvableByAlgorithmX());
        AlgorithmX::printDataStructure(sparseMatrix);

        CHECK(AlgorithmX::hasUniqueSolution(sparseMatrix, seed));
        const std::vector<std::unordered_set<int32_t>> allSolutions = AlgorithmX::findAllSolutions(sparseMatrix, seed);
        CHECK(allSolutions.size() == 1);
        CHECK(allSolutions.at(0) == exactCoverSolution);
        const std::unordered_set<int32_t> oneSolution = AlgorithmX::findOneSolution(sparseMatrix, seed);
        CHECK(!oneSolution.empty());
        CHECK(oneSolution == exactCoverSolution);
      }
    }
  }

  SUBCASE("Single solution, primary and secondary items") {
    const std::vector<
        std::tuple<std::vector<std::vector<bool>>, std::unordered_set<int32_t>, std::unordered_set<int32_t>>>
        exactCoverMatrices = {
            {
                // Example 1
                {
                    {1, 0, 0, 0, 0, 0, 0}, // Part of solution 1
                    {0, 1, 0, 0, 1, 0, 0}, // Part of solution 1
                    {0, 1, 0, 0, 0, 1, 0},
                    {0, 0, 1, 0, 1, 1, 0},
                    {0, 0, 1, 0, 0, 1, 1},
                    {0, 0, 1, 0, 0, 1, 0}, // Part of solution 1
                    {0, 0, 0, 1, 0, 0, 1}, // Part of solution 1
                },
                {4, 5, 6}, // Indices of the items that are secondary
                {0, 1, 5, 6}, // Solution 1
            },
            {
                // Example 2
                {
                    {0, 1, 0, 1},
                    {0, 1, 0, 0}, // Part of solution 2
                    {1, 0, 0, 0}, // Part of solution 2
                    {0, 0, 0, 0},
                    {0, 0, 1, 1}, // Part of solution 2
                    {1, 0, 0, 1},
                },
                {3}, // Indices of the items that are secondary
                {1, 2, 4}, // Solution 2
            },
            {
                // Example 3 (like Example 2, but with the secondary colum moved)
                {
                    {1, 0, 1, 0},
                    {0, 0, 1, 0}, // Part of solution 3
                    {0, 1, 0, 0}, // Part of solution 3
                    {0, 0, 0, 0},
                    {1, 0, 0, 1}, // Part of solution 3
                    {1, 1, 0, 0},
                },
                {0}, // Indices of the items that are secondary
                {1, 2, 4}, // Solution 2
            },
            {
                // Example 4 (empty secondary items)
                {
                    {1, 0, 0, 0, 0, 0}, // Part of solution 4
                    {0, 0, 1, 0, 0, 0},
                    {0, 0, 0, 1, 0, 0}, // Part of solution 4
                    {0, 0, 1, 1, 0, 0},
                    {0, 1, 1, 0, 0, 0}, // Part of solution 4
                },
                {4, 5}, // Indices of the items that are secondary
                {0, 2, 4}, // Solution 4
            },
            {
                // Example 5
                {
                    {0, 0, 0, 1, 0}, //  No primary items! This solution is not selected
                    {1, 0, 1, 0, 0}, //  Part of solution 5
                    {0, 0, 0, 0, 0},
                    {0, 1, 0, 0, 1}, //  Part of solution 5
                },
                {2, 3, 4}, // Indices of the items that are secondary
                {1, 3}, // Solution 5
            },
        };
    for (const auto& seed : seeds) {
      for (const auto& [exactCoverMatrix, secondaryItemIndices, exactCoverSolution] : exactCoverMatrices) {
        SparseCoordinateMatrix sparseMatrix(exactCoverMatrix, secondaryItemIndices);
        REQUIRE(sparseMatrix.isSolvableByAlgorithmX());
        //   AlgorithmX::printDataStructure(sparseMatrix);
        CHECK(AlgorithmX::hasUniqueSolution(sparseMatrix, seed));
        const std::vector<std::unordered_set<int32_t>> allSolutions = AlgorithmX::findAllSolutions(sparseMatrix, seed);
        CHECK(allSolutions.size() == 1);
        CHECK(allSolutions.at(0) == exactCoverSolution);
        const std::unordered_set<int32_t> oneSolution = AlgorithmX::findOneSolution(sparseMatrix, seed);
        CHECK(!oneSolution.empty());
        CHECK(oneSolution == exactCoverSolution);
      }
    }
  }

  SUBCASE("Multiple solutions, primary items only") {
    const std::vector<std::pair<std::vector<std::vector<bool>>, std::vector<std::unordered_set<int32_t>>>>
        exactCoverMatrices = {
            {
                // Example 1
                {
                    {1, 0, 0, 1, 0, 0, 1},
                    {1, 0, 0, 1, 0, 1, 0}, // Part of solution 1.A
                    {0, 1, 0, 0, 1, 1, 0}, // Part of solution 1.B
                    {0, 1, 1, 0, 0, 1, 0},
                    {0, 0, 1, 1, 0, 0, 0}, // Part of solution 1.B
                    {0, 0, 1, 0, 1, 0, 0}, // Part of solution 1.A
                    {0, 1, 0, 0, 0, 0, 1}, // Part of solution 1.A
                    {0, 0, 0, 1, 1, 0, 1},
                    {0, 0, 0, 0, 0, 0, 0}, // Empty option
                    {1, 0, 0, 0, 0, 0, 1}, // Part of solution 1.B
                    {0, 0, 1, 0, 0, 1, 0},
                },
                {
                    {1, 5, 6}, // Solution 1.A
                    {2, 4, 9}, // Solution 1.B
                },
            },
            {
                // Example 2
                {
                    {1, 0, 0, 0}, // Part of solution 2.A | 2.B
                    {0, 1, 0, 0}, // Part of solution 2.A
                    {0, 0, 1, 0}, // Part of solution 2.A | 2.C
                    {0, 0, 0, 1}, // Part of solution 2.A | 2.B | 2.C
                    {0, 1, 1, 0}, // Part of solution 2.B
                    {1, 1, 0, 0}, // Part of solution 2.C
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
                    {0, 0, 0}, // Empty option
                    {1, 0, 0}, // Solution 3.A | 3.B
                    {0, 1, 0}, // Solution 3.A | 3.C
                    {0, 0, 1}, // Solution 3.A | 3.D
                    {1, 1, 0}, // Solution 3.D
                    {1, 0, 1}, // Solution 3.C
                    {0, 1, 1}, // Solution 3.B
                    {1, 1, 1}, // Solution 3.E
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
                    {1, 0}, // Solution 4.A | 4.B | 4.C
                    {1, 0}, // Solution 4.D | 4.E | 4.F
                    {1, 0}, // Solution 4.G | 4.H | 4.I
                    {0, 1}, // Solution 4.A | 4.D | 4.G
                    {0, 1}, // Solution 4.B | 4.E | 4.H
                    {0, 1}, // Solution 4.C | 4.F | 4.I
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
                    {1}, // Solution 5.A
                    {1}, // Solution 5.B
                    {1}, // Solution 5.C
                },
                {
                    {0}, // Solution 5.A
                    {1}, // Solution 5.B
                    {2}, // Solution 5.C
                },
            },
        };
    for (const auto& seed : seeds) {
      for (const auto& [exactCoverMatrix, exactCoverSolutions] : exactCoverMatrices) {
        SparseCoordinateMatrix sparseMatrix(exactCoverMatrix);
        REQUIRE(sparseMatrix.isSolvableByAlgorithmX());
        //   AlgorithmX::printDataStructure(sparseMatrix);
        CHECK(!AlgorithmX::hasUniqueSolution(sparseMatrix, seed));
        const std::vector<std::unordered_set<int32_t>> allSolutions = AlgorithmX::findAllSolutions(sparseMatrix, seed);
        // Check that every solution found appears in the list of the ones provided
        CHECK(allSolutions.size() == exactCoverSolutions.size());
        for (const auto& solution : allSolutions) {
          CHECK(std::count(exactCoverSolutions.begin(), exactCoverSolutions.end(), solution) == 1);
        }
        // Check that the one solution found appears in the list of the ones provided
        const std::unordered_set<int32_t> oneSolution = AlgorithmX::findOneSolution(sparseMatrix, seed);
        CHECK(!oneSolution.empty());
        CHECK(std::count(exactCoverSolutions.begin(), exactCoverSolutions.end(), oneSolution) == 1);
      }
    }
  }

  SUBCASE("Multiple solutions, primary and secondary items") {
    const std::vector<std::tuple<std::vector<std::vector<bool>>,
                                 std::unordered_set<int32_t>,
                                 std::vector<std::unordered_set<int32_t>>>>
        exactCoverMatrices = {
            {
                // Example 1
                {
                    {0, 0, 1, 0, 1, 0, 0}, // Part of solution 1.A
                    {1, 0, 0, 1, 0, 0, 1}, // Part of solution 1.B
                    {0, 1, 1, 0, 0, 1, 0}, // Part of solution 1.B
                    {1, 0, 0, 1, 0, 1, 0}, // Part of solution 1.A
                    {0, 1, 0, 0, 0, 0, 1}, // Part of solution 1.A
                    {0, 0, 0, 1, 1, 0, 1},
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
                    {0, 0, 1, 0}, // Part of solution 2.A
                    {1, 0, 0, 1}, // Part of solution 2.B
                    {0, 1, 1, 0}, // Part of solution 2.B
                    {0, 0, 0, 0},
                    {1, 1, 0, 0}, // Part of solution 2.A
                },
                {3}, // Indices of the items that are secondary
                {
                    {0, 4}, // Solution 2.A
                    {1, 2}, // Solution 2.B
                },
            },
            // {
            //     // Example 3 (like Example 2, but with the secondary colum moved)
            //     {
            //         {-1, -1, -1, 10}, // Part of solution 3.A
            //         {12, 13, -1, -1}, // Part of solution 3.B
            //         {-1, -1, 15, 16}, // Part of solution 3.B
            //         {-1, -1, -1, -1},
            //         {14, -1, 15, -1}, // Part of solution 3.A
            //     },
            //     {1}, // Indices of the items that are secondary
            //     {
            //         {0, 4}, // Solution 3.A
            //         {1, 2}, // Solution 3.B
            //     },
            // },
            {
                // Example 4
                {
                    {0, 1, 0, 0, 0}, //  Part of solution 4.A
                    {1, 0, 1, 0, 1}, //  Part of solution 4.A | 4.B | 4.C
                    {0, 0, 0, 0, 0},
                    {0, 1, 0, 1, 1},
                    {0, 1, 0, 1, 0}, //  Part of solution 4.B
                    {0, 1, 0, 0, 0}, //  Part of solution 4.C
                },
                {3, 4}, // Indices of the items that are secondary
                {
                    {0, 1}, // Solution 4.A
                    {1, 4}, // Solution 4.B
                    {1, 5}, // Solution 4.C
                },
            },
        };
    for (const auto& seed : seeds) {
      for (const auto& [exactCoverMatrix, secondaryItemIndices, exactCoverSolutions] : exactCoverMatrices) {
        SparseCoordinateMatrix sparseMatrix(exactCoverMatrix, secondaryItemIndices);
        REQUIRE(sparseMatrix.isSolvableByAlgorithmX());
        //   AlgorithmX::printDataStructure(sparseMatrix);
        CHECK(!AlgorithmX::hasUniqueSolution(sparseMatrix, seed));
        const std::vector<std::unordered_set<int32_t>> allSolutions = AlgorithmX::findAllSolutions(sparseMatrix, seed);
        // Check that every solution found appears in the list of the ones provided
        CHECK(allSolutions.size() == exactCoverSolutions.size());
        for (const auto& solution : allSolutions) {
          CHECK(std::count(exactCoverSolutions.begin(), exactCoverSolutions.end(), solution) == 1);
        }
        // Check that the one solution found appears in the list of the ones provided
        const std::unordered_set<int32_t> oneSolution = AlgorithmX::findOneSolution(sparseMatrix, seed);
        CHECK(!oneSolution.empty());
        CHECK(std::count(exactCoverSolutions.begin(), exactCoverSolutions.end(), oneSolution) == 1);
      }
    }
  }
}
