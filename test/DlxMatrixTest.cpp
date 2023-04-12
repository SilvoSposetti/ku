#include "solver/DlxMatrix.h"

#include "doctest.h"
#include "solver/SparseCoordinateMatrix.h"

SparseCoordinateMatrix createSparseCoordinateMatrix(const std::vector<std::vector<int32_t>>& matrix) {
  const int32_t rows = matrix.size();
  const int32_t columns = matrix[0].size();
  SparseCoordinateMatrix sparseMatrix(rows, columns);
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      sparseMatrix.setCell(i, j, matrix[i][j]);
    }
  }
  CHECK(sparseMatrix.isSolvableByDlx());
  return sparseMatrix;
}

TEST_CASE("DLX Matrix") {
  SUBCASE("No solutions") {}

  SUBCASE("Single solution with only primary items") {
    const std::vector<std::vector<int32_t>> exactCoverMatrix = {
        {-1, -1, 10, -1, 11, -1, -1}, // This option is part of the unique solution
        {12, -1, -1, 13, -1, -1, 14},
        {-1, 15, 16, -1, -1, 17, -1},
        {18, -1, -1, 19, -1, 20, -1}, // This option is part of the unique solution
        {-1, 21, -1, -1, -1, -1, 22}, // This option is part of the unique solution
        {-1, -1, -1, 23, 24, -1, 25},
    };

    SparseCoordinateMatrix sparseMatrix = createSparseCoordinateMatrix(exactCoverMatrix);
    DlxMatrix dlxMatrix(sparseMatrix);
    // dlxMatrix.printDataStructure();

    const std::unordered_set<int32_t> solutionRows = dlxMatrix.solve();
    CHECK(solutionRows == std::unordered_set<int32_t>{0, 3, 4});
  }

  SUBCASE("Single solutions with secondary items") {}
  SUBCASE("Multiple solutions") {}
  SUBCASE("Multiple solutions with secondary items") {}
}
