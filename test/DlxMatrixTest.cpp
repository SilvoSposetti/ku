#include "solver/DlxMatrix.h"

#include "doctest.h"
#include "solver/SparseCoordinateMatrix.h"

TEST_CASE("DLX Matrix") {
  SUBCASE("No solutions") {
    const std::vector<std::vector<std::vector<int32_t>>> unsolvableMatrices = {
        {
            // Invalid (fully-empty) matrix
            {-1, -1, -1, -1, -1, -1, -1},
            {-1, -1, -1, -1, -1, -1, -1},
            {-1, -1, -1, -1, -1, -1, -1},
        },
        {
            // Valid, but no solution possible
            {1, -1, 1},
            {-1, 1, 1},
            {1, 1, -1},
            {-1, -1, -1},
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
            {-1, -1, -1, -1, -1, -1, -1},
            {23, 24, 25, -1, -1, -1, -1},

        },
    };
    for (const auto& unsolvableMatrix : unsolvableMatrices) {

      SparseCoordinateMatrix sparseMatrix(unsolvableMatrix);
      // CHECK(sparseMatrix.isSolvableByDlx()); // Theoretically solvable, but no solutions can be found
      DlxMatrix dlxMatrix(sparseMatrix);
      dlxMatrix.printDataStructure();

      const std::unordered_set<int32_t> solutionRows = dlxMatrix.solve();
      CHECK(solutionRows.empty());
    }
  }

  SUBCASE("Single solution with only primary items") {
    const std::vector<std::vector<int32_t>> exactCoverMatrix = {
        {-1, -1, 10, -1, 11, -1, -1}, // This option [0] is part of the unique solution
        {12, -1, -1, 13, -1, -1, 14},
        {-1, 15, 16, -1, -1, 17, -1},
        {18, -1, -1, 19, -1, 20, -1}, // This option [3] is part of the unique solution
        {-1, 21, -1, -1, -1, -1, 22}, // This option [4] is part of the unique solution
        {-1, -1, -1, 23, 24, -1, 25},
    };

    SparseCoordinateMatrix sparseMatrix(exactCoverMatrix);
    CHECK(sparseMatrix.isSolvableByDlx());
    DlxMatrix dlxMatrix(sparseMatrix);
    // dlxMatrix.printDataStructure();

    const std::unordered_set<int32_t> solutionRows = dlxMatrix.solve();
    CHECK(solutionRows == std::unordered_set<int32_t>{0, 3, 4});
  }

  SUBCASE("Single solutions with secondary items") {}
  SUBCASE("Multiple solutions") {}
  SUBCASE("Multiple solutions with secondary items") {}
}
