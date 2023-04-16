#include "solver/DlxMatrix.h"

#include "doctest.h"
#include "solver/SparseCoordinateMatrix.h"

TEST_CASE("DLX Matrix") {
  SUBCASE("No solutions") {
    const std::vector<std::vector<std::vector<int32_t>>> unsolvableMatrices = {
        {
            // Empty matrix
        },
        {
            // Invalid (fully-empty) matrix
            {-1, -1, -1, -1, -1, -1, -1},
            {-1, -1, -1, -1, -1, -1, -1},
            {-1, -1, -1, -1, -1, -1, -1},
        },
        {
            // Valid, but no solution possible
            {-1, -1, -1}, // Empty option
            {1, -1, 1},
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
    };
    for (const auto& unsolvableMatrix : unsolvableMatrices) {

      SparseCoordinateMatrix sparseMatrix(unsolvableMatrix);
      // CHECK(sparseMatrix.isSolvableByDlx()); // Theoretically solvable, but no solutions can be found
      DlxMatrix dlxMatrix(sparseMatrix);
      // dlxMatrix.printDataStructure();

      const std::unordered_set<int32_t> solutionRows = dlxMatrix.solve();
      CHECK(solutionRows.empty());
    }
  }

  SUBCASE("Single solution, primary items only") {
    const std::vector<std::pair<std::vector<std::vector<int32_t>>, std::unordered_set<int32_t>>> exactCoverMatrices = {
        {{
             // Example 1
             {-1, -1, 10, -1, 11, -1, -1}, // This option [0] is part of the unique solution
             {12, -1, -1, 13, -1, -1, 14},
             {-1, 15, 16, -1, -1, 17, -1},
             {18, -1, -1, 19, -1, 20, -1}, // This option [3] is part of the unique solution
             {-1, 21, -1, -1, -1, -1, 22}, // This option [4] is part of the unique solution
             {-1, -1, -1, 23, 24, -1, 25},
         },
         {0, 3, 4}}, // Solution 1
        {{
             // Example 2
             {10, -1, -1, 11, -1, -1, 12},
             {13, -1, -1, 14, -1, -1, -1}, // This option [1] is part of the unique solution
             {-1, -1, -1, 15, 16, -1, 17},
             {-1, -1, 18, -1, 19, 20, -1}, // This option [3] is part of the unique solution
             {-1, 21, 22, -1, -1, 23, 24},
             {-1, 25, -1, -1, -1, -1, 30}, // This option [5] is part of the unique solution
             {-1, -1, -1, -1, -1, -1, -1}, // Empty option

         },
         {1, 3, 5}}, // Solution 2
        {{
             // Example 3
             {10, 11, -1, -1, -1, -1}, // This option [0] is part of the unique solution
             {-1, -1, -1, -1, 12, 13}, // This option [1] is part of the unique solution
             {-1, -1, -1, 14, 15, -1},
             {16, -1, 18, -1, -1, -1},
             {-1, -1, -1, -1, -1, -1}, // Empty option
             {-1, -1, 21, -1, -1, -1}, // This option [5] is part of the unique solution
             {-1, -1, -1, 22, -1, -1}, // This option [6] is part of the unique solution
             {23, -1, 24, -1, -1, 26},
         },
         {0, 1, 5, 6}}, // Solution 3
    };

    for (const auto& [exactCoverMatrix, solution] : exactCoverMatrices) {
      SparseCoordinateMatrix sparseMatrix(exactCoverMatrix);
      CHECK(sparseMatrix.isSolvableByDlx());
      DlxMatrix dlxMatrix(sparseMatrix);
      // dlxMatrix.printDataStructure();

      const std::unordered_set<int32_t> solutionRows = dlxMatrix.solve();
      CHECK(solutionRows == solution);
    }
  }

  SUBCASE("Single solution, primary and secondary items") {}
  SUBCASE("Multiple solutions, primary items only") {}
  SUBCASE("Multiple solutions, primary and secondary items") {}
}
