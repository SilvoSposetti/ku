#include "solver/DlxMatrix.h"

#include "solver/SparseCoordinateMatrix.h"

#include <doctest.h>

TEST_CASE("DLX Matrix") {

  SUBCASE("Basic usage") {
    const std::vector<std::vector<int32_t>> exactCoverMatrix = {
        {-1, -1, 65, -1, 12, -1, -1},
        {2, -1, -1, 8, -1, -1, 9},
        {-1, 70, 3, -1, -1, 19, -1},
        {11, -1, -1, 4, -1, 98, -1},
        {-1, 27, -1, -1, -1, -1, 6},
        {-1, -1, -1, 9, 68, -1, 13},
    };
    const int32_t rows = exactCoverMatrix.size();
    const int32_t columns = exactCoverMatrix.at(0).size();
    SparseCoordinateMatrix sparseMatrix(rows, columns);
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < columns; j++) {
        sparseMatrix.setCell(i, j, exactCoverMatrix[i][j]);
      }
    }

    DlxMatrix matrix{sparseMatrix};

    // Cover first column
    matrix.coverColumn(0);

  }
  SUBCASE("Full example") {}
}
