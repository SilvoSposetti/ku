
#include "solver/SparseCoordinateMatrix.h"

#include <doctest.h>

TEST_CASE("SparseCoordinateMatrixTest") {

  SUBCASE("Basic Usage") {
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
    CHECK(sparseMatrix.getRowsAmount() == rows);
    CHECK(sparseMatrix.getColumnsAmount() == columns);

    // Set primary / secondary columns
    sparseMatrix.setColumnSecondary(0);
    sparseMatrix.setColumnSecondary(3);
    CHECK_FALSE(sparseMatrix.isColumnPrimary(0));
    CHECK(sparseMatrix.isColumnPrimary(1));
    CHECK(sparseMatrix.isColumnPrimary(2));
    CHECK_FALSE(sparseMatrix.isColumnPrimary(3));
    CHECK(sparseMatrix.isColumnPrimary(4));
    CHECK(sparseMatrix.isColumnPrimary(5));

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

  SUBCASE("Reorder") {
    // Create & fill
    const std::vector<std::vector<int32_t>> matrix = {
        {0, 1, 2, 3, 4, 5},
        {11, 0, 13, 0, 0, 0},
        {21, 22, 0, 24, 0, 0},
        {0, 32, 33, 0, 0, 0},
        {0, 0, 43, 44, 0, 46},
        {0, 0, 0, 0, 0, 0},
        {61, 62, 0, 0, 0, 66},
    };
    const std::vector<std::vector<int32_t>> reorderedMatrix = {
        {5, 3, 1, 2, 0, 4},
        {0, 0, 0, 13, 11, 0},
        {0, 24, 22, 0, 21, 0},
        {0, 0, 32, 33, 0, 0},
        {46, 44, 0, 43, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {66, 0, 62, 0, 61, 0},
    };

    const std::vector<int32_t> newPermutation = {5, 3, 1, 2, 0, 4};
    const std::vector<int32_t> newPermutationWrongIndices = {5, 9, 1, 2, 0, 4};
    const std::vector<int32_t> newPermutationDuplicateIndices = {5, 2, 1, 2, 0, 4};
    const std::vector<int32_t> newPermutationTooBig = {5, 3, 1, 2, 0, 4, 9};
    const std::vector<int32_t> newPermutationTooSmall = {5, 1, 2, 0, 4};

    const int32_t rows = matrix.size();
    const int32_t columns = matrix.at(0).size();
    SparseCooordinateMatrix sparseMatrix(rows, columns);
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < columns; j++) {
        CHECK(sparseMatrix.setData(i, j, matrix[i][j]));
      }
    }

    // Input check
    CHECK_FALSE(sparseMatrix.reorderColumns(newPermutationTooBig));
    CHECK_FALSE(sparseMatrix.reorderColumns(newPermutationTooSmall));
    CHECK_FALSE(sparseMatrix.reorderColumns(newPermutationDuplicateIndices));
    CHECK_FALSE(sparseMatrix.reorderColumns(newPermutationWrongIndices));

    // Reorder
    CHECK(sparseMatrix.reorderColumns(newPermutation));

    // Get reordered matrix elements one by one and confirm that they match the reordered matrix
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < columns; j++) {
        CHECK(reorderedMatrix.at(i).at(j) == sparseMatrix.getData(i, j));
      }
    }
  }
}
