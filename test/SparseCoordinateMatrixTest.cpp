
#include "solver/SparseCoordinateMatrix.h"

#include <doctest.h>

TEST_CASE("SparseCoordinateMatrixTest") {

  SUBCASE("Create, Fill, Read") {
    // Create & fill
    const std::vector<std::vector<int32_t>> matrix = {
        {11, 0, 13, 0, 0, 0},
        {21, 22, 0, 24, 0, 0},
        {0, 32, 33, 0, 0, 0},
        {0, 0, 43, 44, 0, 46},
        {0, 0, 0, 0, 0, 0},
        {61, 62, 0, 0, 0, 66},
    };
    const int32_t rows = matrix.size();
    const int32_t columns = matrix.at(0).size();
    SparseCooordinateMatrix sparseMatrix(rows, columns);
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < columns; j++) {
        CHECK(sparseMatrix.setData(i, j, matrix[i][j]));
      }
    }

    // Cannot set data outside of matrix size
    CHECK_FALSE(sparseMatrix.setData(rows, 0, 25));
    CHECK_FALSE(sparseMatrix.setData(0, columns + 25, 25));
    CHECK_FALSE(sparseMatrix.setData(-5, 0, 25));
    CHECK_FALSE(sparseMatrix.setData(0, -1, 25));

    // Retrieving element outside of the sparse matrix returns 0
    CHECK(sparseMatrix.getData(-1, 0) == 0);
    CHECK(sparseMatrix.getData(0, -1) == 0);
    CHECK(sparseMatrix.getData(rows, 0) == 0);
    CHECK(sparseMatrix.getData(0, columns) == 0);

    // Get matrix elements one by one and confirm that they match the source matrix
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < columns; j++) {
        CHECK(matrix.at(i).at(j) == sparseMatrix.getData(i, j));
      }
    }
  }
}
