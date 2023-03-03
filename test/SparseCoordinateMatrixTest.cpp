
#include "solver/SparseCoordinateMatrix.h"

#include <doctest.h>
#include <unordered_map>

TEST_CASE("SparseCoordinateMatrixTest") {

  SUBCASE("Basic Usage") {
    // Create & fill
    const std::vector<std::vector<int32_t>> matrix = {
        {11, -1, 13, -1, -1, -1},
        {21, 22, -1, 24, -1, -1},
        {-1, 32, 33, -1, -1, -1},
        {-1, -1, 43, 44, -1, 46},
        {-1, -1, -1, -1, -1, -1},
        {61, 62, -1, -1, -1, 66},
    };
    const int32_t rows = matrix.size();
    const int32_t columns = matrix.at(0).size();
    SparseCooordinateMatrix sparseMatrix(rows, columns);
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < columns; j++) {
        CHECK(sparseMatrix.setCell(i, j, matrix[i][j]));
      }
    }
    CHECK(sparseMatrix.getRowsAmount() == rows);
    CHECK(sparseMatrix.getColumnsAmount() == columns);

    // Cannot set data outside of matrix size
    CHECK_FALSE(sparseMatrix.setCell(rows, 0, 25));
    CHECK_FALSE(sparseMatrix.setCell(0, columns + 25, 25));
    CHECK_FALSE(sparseMatrix.setCell(-5, 0, 25));
    CHECK_FALSE(sparseMatrix.setCell(0, -1, 25));

    // Retrieving element outside of the sparse matrix returns false
    CHECK(sparseMatrix.getCell(-1, 0) < 0);
    CHECK(sparseMatrix.getCell(0, -1) < 0);
    CHECK(sparseMatrix.getCell(rows, 0) < 0);
    CHECK(sparseMatrix.getCell(0, columns) < 0);

    // Get matrix elements one by one and confirm that they match the source matrix
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < columns; j++) {
        CHECK(matrix.at(i).at(j) == sparseMatrix.getCell(i, j));
      }
    }

    // Modify sparse matrix by toggling the data in some specific cells
    const std::vector<std::pair<int32_t, int32_t>> indices = {{1, 5}, {2, 2}, {3, 2}, {3, 0}, {4, 1}};
    for (const auto& [i, j] : indices) {
      const int32_t value = matrix[i][j];
      int32_t newValue = -1;
      if (value < 0) {
        newValue = 5;
      }
      CHECK(sparseMatrix.setCell(i, j, newValue));
      CHECK(sparseMatrix.getCell(i, j) != matrix[i][j]);
    }
    // Re-set them back and check again the whole matrix
    for (const auto& [i, j] : indices) {
      CHECK(sparseMatrix.setCell(i, j, matrix[i][j]));
    }
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < columns; j++) {
        CHECK(matrix.at(i).at(j) == sparseMatrix.getCell(i, j));
      }
    }
  }

  SUBCASE("Reorder") {
    // Create & fill
    const std::vector<std::vector<int32_t>> matrix = {
        {0, 1, 2, 3, 4, 5},
        {11, -1, 13, -1, -1, -1},
        {21, 22, -1, 24, -1, -1},
        {-1, 32, 33, -1, -1, -1},
        {-1, -1, 43, 44, -1, 46},
        {-1, -1, -1, -1, -1, -1},
        {61, 62, -1, -1, -1, 66},
    };
    const std::vector<std::vector<int32_t>> reorderedMatrix = {
        {5, 3, 1, 2, 0, 4},
        {-1, -1, -1, 13, 11, -1},
        {-1, 24, 22, -1, 21, -1},
        {-1, -1, 32, 33, -1, -1},
        {46, 44, -1, 43, -1, -1},
        {-1, -1, -1, -1, -1, -1},
        {66, -1, 62, -1, 61, -1},
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
        CHECK(sparseMatrix.setCell(i, j, matrix[i][j]));
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
        CHECK(reorderedMatrix.at(i).at(j) == sparseMatrix.getCell(i, j));
      }
    }
  }

  SUBCASE("Dlx-Solvability") {
    // Create & fill. Note that there's no column with all cells unset
    const std::vector<std::vector<int32_t>> matrix = {
        {11, -1, 13, -1, -1, -1},
        {21, 22, -1, 24, -1, -1},
        {-1, 32, 33, -1, 5, -1},
        {-1, -1, 43, 44, -1, 46},
        {-1, -1, -1, -1, -1, -1},
        {61, 62, -1, -1, -1, 66},
    };
    const int32_t rows = matrix.size();
    const int32_t columns = matrix.at(0).size();
    SparseCooordinateMatrix sparseMatrix(rows, columns);
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < columns; j++) {
        CHECK(sparseMatrix.setCell(i, j, matrix[i][j]));
      }
    }
    // All columns are primary by default
    CHECK(sparseMatrix.isSolvableByDlx());

    {
      // Sparse matrix is not solvable by DLX if all columns are secondary
      SparseCooordinateMatrix allSecondaryColumnsSparseMatrix = sparseMatrix;
      for (int j = 0; j < columns; j++) {
        allSecondaryColumnsSparseMatrix.setColumnSecondary(j);
      }
      CHECK_FALSE(allSecondaryColumnsSparseMatrix.isSolvableByDlx());
    }

    // Set primary / secondary columns
    sparseMatrix.setColumnSecondary(0);
    sparseMatrix.setColumnSecondary(3);
    CHECK_FALSE(sparseMatrix.isColumnPrimary(0));
    CHECK(sparseMatrix.isColumnPrimary(1));
    CHECK(sparseMatrix.isColumnPrimary(2));
    CHECK_FALSE(sparseMatrix.isColumnPrimary(3));
    CHECK(sparseMatrix.isColumnPrimary(4));
    CHECK(sparseMatrix.isColumnPrimary(5));

    // Even after modifications, the matrix is still solvable
    CHECK(sparseMatrix.isSolvableByDlx());

    // Set the only cell of column index 4 of the matrix to false
    sparseMatrix.setCell(2, 4, -1);
    // Now matrix is unsolvable
    CHECK_FALSE(sparseMatrix.isSolvableByDlx());

    // But if that column is set to be secondary
    sparseMatrix.setColumnSecondary(4);
    // Then it can be solved
    CHECK(sparseMatrix.isSolvableByDlx());
  }
}
