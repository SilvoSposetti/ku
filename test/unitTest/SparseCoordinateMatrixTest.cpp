
#include "solver/SparseCoordinateMatrix.h"

#include <doctest.h>
#include <unordered_map>

TEST_CASE("Sparse Coordinate Matrix") {

  SUBCASE("Basic Usage") {
    // Create & fill
    const std::vector<std::vector<bool>> matrix = {
        {1, 0, 1, 0, 0, 0},
        {1, 1, 0, 1, 0, 0},
        {0, 1, 1, 0, 0, 0},
        {0, 0, 1, 1, 0, 1},
        {0, 0, 0, 0, 0, 0},
        {1, 1, 0, 0, 0, 1},
    };
    const std::vector<int32_t> matrixData = {1, 2, 3, 4, 5, 6};
    const int32_t rows = matrix.size();
    const int32_t columns = matrix.at(0).size();
    SparseCoordinateMatrix sparseMatrix(matrix, {}, matrixData);
    CHECK(sparseMatrix.getRowsAmount() == rows);
    CHECK(sparseMatrix.getColumnsAmount() == columns);
    CHECK(sparseMatrix.getValidElementsAmount() == 13);

    // Retrieving element outside of the sparse matrix returns false
    CHECK_FALSE(sparseMatrix.isCellSet(-1, 0));
    CHECK_FALSE(sparseMatrix.isCellSet(0, -1));
    CHECK_FALSE(sparseMatrix.isCellSet(rows, 0));
    CHECK_FALSE(sparseMatrix.isCellSet(0, columns));

    // Get matrix elements one by one and confirm that they match the source matrix
    for (int32_t rowId = 0; rowId < rows; rowId++) {
      CHECK(sparseMatrix.getRowData(rowId) == matrixData[rowId]);
    }
  }

  SUBCASE("Reorder") {
    // Create & fill
    const std::vector<std::vector<bool>> matrix = {
        // {0, 1, 2, 3, 4, 5},
        {1, 0, 1, 0, 0, 0},
        {1, 1, 0, 1, 0, 0},
        {0, 1, 1, 0, 0, 0},
        {0, 0, 1, 1, 0, 1},
        {0, 0, 0, 0, 0, 0},
        {1, 1, 0, 0, 0, 1},
    };
    const std::vector<std::vector<bool>> reorderedMatrix = {
        // {5, 3, 1, 2, 0, 4},
        {0, 0, 0, 1, 1, 0},
        {0, 1, 1, 0, 1, 0},
        {0, 0, 1, 1, 0, 0},
        {1, 1, 0, 1, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {1, 0, 1, 0, 1, 0},
    };

    const std::vector<int32_t> newPermutation = {5, 3, 1, 2, 0, 4};
    const std::vector<int32_t> newPermutationWrongIndices = {5, 9, 1, 2, 0, 4};
    const std::vector<int32_t> newPermutationDuplicateIndices = {5, 2, 1, 2, 0, 4};
    const std::vector<int32_t> newPermutationTooBig = {5, 3, 1, 2, 0, 4, 9};
    const std::vector<int32_t> newPermutationTooSmall = {5, 1, 2, 0, 4};

    const int32_t rows = matrix.size();
    const int32_t columns = matrix.at(0).size();
    SparseCoordinateMatrix sparseMatrix(matrix);

    // Input check
    CHECK_FALSE(sparseMatrix.reorderColumns(newPermutationTooBig));
    CHECK_FALSE(sparseMatrix.reorderColumns(newPermutationTooSmall));
    CHECK_FALSE(sparseMatrix.reorderColumns(newPermutationDuplicateIndices));
    CHECK_FALSE(sparseMatrix.reorderColumns(newPermutationWrongIndices));

    // Confirm that the matrix was untouched
    for (int32_t i = 0; i < rows; i++) {
      for (int32_t j = 0; j < columns; j++) {
        CHECK(matrix[i][j] == sparseMatrix.isCellSet(i, j));
      }
    }

    // Reorder
    CHECK(sparseMatrix.reorderColumns(newPermutation));

    // Get reordered matrix elements one by one and confirm that they match the reordered matrix
    for (int32_t i = 0; i < rows; i++) {
      for (int32_t j = 0; j < columns; j++) {
        CHECK(reorderedMatrix[i][j] == sparseMatrix.isCellSet(i, j));
      }
    }
  }

  SUBCASE("Algorithm X Solvability") {
    // Create & fill. Note that there's no column with all cells unset
    const std::vector<std::vector<bool>> matrix = {
        {1, 0, 1, 0, 0, 0},
        {1, 1, 0, 1, 0, 0},
        {0, 1, 1, 0, 1, 0},
        {0, 0, 1, 1, 0, 1},
        {0, 0, 0, 0, 0, 0},
        {1, 1, 0, 0, 0, 1},
    };
    const int32_t columns = matrix.at(0).size();
    SparseCoordinateMatrix sparseMatrix(matrix);

    // All columns are primary by default
    CHECK(sparseMatrix.isSolvableByAlgorithmX());

    {
      // Sparse matrix is not solvable by Algorithm X if all columns are secondary
      std::unordered_set<int32_t> allColumns;
      for (int32_t columnId = 0; columnId < columns; columnId++) {
        allColumns.insert(columnId);
      }
      SparseCoordinateMatrix allSecondaryColumnsSparseMatrix(matrix, allColumns);
      CHECK_FALSE(allSecondaryColumnsSparseMatrix.isSolvableByAlgorithmX());
    }

    {
      // With the given modifications, the matrix is still solvable
      SparseCoordinateMatrix solvableSparseMatrix(matrix, {0, 3});
      CHECK_FALSE(solvableSparseMatrix.isColumnPrimary(0));
      CHECK(solvableSparseMatrix.isColumnPrimary(1));
      CHECK(solvableSparseMatrix.isColumnPrimary(2));
      CHECK_FALSE(solvableSparseMatrix.isColumnPrimary(3));
      CHECK(solvableSparseMatrix.isColumnPrimary(4));
      CHECK(solvableSparseMatrix.isColumnPrimary(5));

      CHECK(solvableSparseMatrix.isSolvableByAlgorithmX());
    }

    {
      // Matrix with an all-zeroes column
      const std::vector<std::vector<bool>> invalidMatrix = {
          {1, 0, 1, 0, 0, 0},
          {1, 1, 0, 1, 0, 0},
          {0, 1, 1, 0, 0, 0},
          {0, 0, 1, 1, 0, 1},
          {0, 0, 0, 0, 0, 0},
          {1, 1, 0, 0, 0, 1},
      };

      SparseCoordinateMatrix unsolvableMatrix(invalidMatrix);
      // Now matrix is unsolvable, since a primary column is empty
      CHECK_FALSE(unsolvableMatrix.isSolvableByAlgorithmX());

      // But if the empty column is set to be secondary
      SparseCoordinateMatrix solvableMatrix(invalidMatrix, {4});
      // Now matrix is unsolvable
      CHECK(solvableMatrix.isSolvableByAlgorithmX());
    }

    // Empty matrix is solvable
    const SparseCoordinateMatrix emptyMatrix({});
    CHECK(emptyMatrix.isSolvableByAlgorithmX());
  }

  SUBCASE("Bulk Constructor") {

    const std::vector<std::tuple<std::vector<std::vector<bool>>, std::unordered_set<int32_t>>> invalidMatrices = {
        {
            {
                // Malformed
                {1, 0, 1, 0, 0, 0},
                {1, 1, 0, 1, 0, 0},
                {0, 1, 1, 0, 0, 0},
                {0, 0, 1, 1, 0, 1, 0}, // This row is malformed
                {0, 0, 0, 0, 0, 0},
                {1, 1, 0, 0, 0, 1},
            },
            {0, 1}, // Secondary column indices
        },
        {
            {
                // Empty matrix
            }, // Empty data vector
            {},
        },
    };
    for (const auto& [invalidMatrix, secondaryColumnsIndices] : invalidMatrices) {
      SparseCoordinateMatrix invalidSparseMatrix(invalidMatrix, secondaryColumnsIndices);
      // 0 Rows or Columns
      CHECK(invalidSparseMatrix.getRowsAmount() == 0);
      CHECK(invalidSparseMatrix.getColumnsAmount() == 0);
      // All elements are invalid
      CHECK(invalidSparseMatrix.getValidElementsAmount() == 0);
      for (std::size_t i = 0; i < invalidMatrix.size(); i++) {
        for (std::size_t j = 0; j < invalidMatrix[i].size(); j++) {
          CHECK_FALSE(invalidSparseMatrix.isCellSet(i, j));
        }
      }
    }

    // Correctly formed
    const std::vector<std::vector<bool>> matrix = {
        {1, 0, 1, 0, 0, 0},
        {1, 1, 0, 1, 0, 0},
        {0, 1, 1, 0, 0, 0},
        {0, 0, 1, 1, 0, 1},
        {0, 0, 0, 0, 0, 0},
        {1, 1, 0, 0, 0, 1},
    };
    const std::vector<int32_t> matrixData = {0, 1, 2, 3, 4, 5};
    const std::unordered_set<int32_t> secondaryColumnIndices = {5, 2};

    SparseCoordinateMatrix sparseMatrix{matrix, secondaryColumnIndices, matrixData};
    CHECK(sparseMatrix.getRowsAmount() == matrix.size());
    CHECK(sparseMatrix.getColumnsAmount() == matrix[0].size());
    CHECK(sparseMatrix.getValidElementsAmount() == 13);
    for (std::size_t i = 0; i < matrix.size(); i++) {
      for (std::size_t j = 0; j < matrix[i].size(); j++) {
        CHECK(sparseMatrix.isCellSet(i, j) == matrix[i][j]);
      }
    }
    for (std::size_t rowIndex = 0; rowIndex < matrixData.size(); rowIndex++) {
      CHECK(sparseMatrix.getRowData(rowIndex) == matrixData[rowIndex]);
    }
    for (std::size_t columnIndex = 0; columnIndex < matrix[0].size(); columnIndex++) {
      if (std::count(secondaryColumnIndices.begin(), secondaryColumnIndices.end(), columnIndex)) {
        CHECK_FALSE(sparseMatrix.isColumnPrimary(columnIndex));
      } else {
        CHECK(sparseMatrix.isColumnPrimary(columnIndex));
      }
    }
  }

  SUBCASE("Column valid cells") {
    const std::vector<std::vector<bool>> matrix = {
        {1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0},
        {1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
        {0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 1},
        {0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0},
        {0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0},
        {1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1},
    };
    const std::vector<int32_t> columnSizes = {3, 6, 2, 4, 4, 3, 3, 1, 1, 1, 5, 5, 1, 2};

    SparseCoordinateMatrix sparseMatrix(matrix);
    for (int32_t columnIndex = 0; columnIndex < sparseMatrix.getColumnsAmount(); columnIndex++) {
      CHECK(columnSizes[columnIndex] == sparseMatrix.getColumnValidCellsAmount(columnIndex));
    }
  }
}
