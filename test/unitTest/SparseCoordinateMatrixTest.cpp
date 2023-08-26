
#include "solver/SparseCoordinateMatrix.h"

#include <algorithm>
#include <doctest.h>
#include <unordered_map>

TEST_CASE("Sparse Coordinate Matrix") {

  SUBCASE("Constuctor") {
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
      SparseCoordinateMatrix sparseMatrix(matrix, {}, false, matrixData);
      CHECK(sparseMatrix.getRowsAmount() == rows);
      CHECK(sparseMatrix.getColumnsAmount() == columns);
      CHECK(sparseMatrix.getValidElementsAmount() == 13);

      // Retrieving element outside of the sparse matrix returns false
      CHECK_FALSE(sparseMatrix.isCellSet(-1, 0));
      CHECK_FALSE(sparseMatrix.isCellSet(0, -1));
      CHECK_FALSE(sparseMatrix.isCellSet(rows, 0));
      CHECK_FALSE(sparseMatrix.isCellSet(0, columns));

      // Get matrix elements one by one and confirm that they match the source matrix. No secondary columns, no
      // reordering happening
      for (int32_t rowIndex = 0; rowIndex < rows; rowIndex++) {
        for (int32_t columnIndex = 0; columnIndex < columns; columnIndex++) {
          CHECK(matrix[rowIndex][columnIndex] == sparseMatrix.isCellSet(rowIndex, columnIndex));
        }
      }

      // Check that the data is consistent
      for (int32_t rowId = 0; rowId < rows; rowId++) {
        CHECK(sparseMatrix.getRowData(rowId) == matrixData[rowId]);
      }
    }

    SUBCASE("Secondary columns at end") {
      // Create & fill
      const std::vector<std::vector<bool>> matrix = {
          // p1, s1, p2, s2, s3, p3
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
      SparseCoordinateMatrix sparseMatrix(matrix, {1, 3, 4}, true, matrixData);
      CHECK(sparseMatrix.getRowsAmount() == rows);
      CHECK(sparseMatrix.getColumnsAmount() == columns);
      CHECK(sparseMatrix.getValidElementsAmount() == 13);

      const std::vector<std::vector<bool>> equivalentReorderedMatrix = {
          // p1, p2, p3, s1, s2, s3
          {1, 1, 0, 0, 0, 0},
          {1, 0, 0, 1, 1, 0},
          {0, 1, 0, 1, 0, 0},
          {0, 1, 1, 0, 1, 0},
          {0, 0, 0, 0, 0, 0},
          {1, 0, 1, 1, 0, 0},
      };
      // Get matrix elements one by one and confirm that they match the equivalent reordered matrix.
      for (int32_t rowIndex = 0; rowIndex < rows; rowIndex++) {
        for (int32_t columnIndex = 0; columnIndex < columns; columnIndex++) {
          CHECK(equivalentReorderedMatrix[rowIndex][columnIndex] == sparseMatrix.isCellSet(rowIndex, columnIndex));
        }
      }

      // Check that the data is consistent
      for (int32_t rowId = 0; rowId < rows; rowId++) {
        CHECK(sparseMatrix.getRowData(rowId) == matrixData[rowId]);
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

    SUBCASE("All columns secondary") {
      // Sparse matrix is not solvable by Algorithm X if all columns are secondary
      std::unordered_set<int32_t> allColumns;
      for (int32_t columnId = 0; columnId < columns; columnId++) {
        allColumns.insert(columnId);
      }
      SparseCoordinateMatrix allSecondaryColumnsSparseMatrix(matrix, allColumns);
      CHECK_FALSE(allSecondaryColumnsSparseMatrix.isSolvableByAlgorithmX());
    }

    SUBCASE("Some columns secondary") {
      // With the given modifications, the matrix is not solvable, as the secondary columns are not at the end
      SparseCoordinateMatrix unsolvableSparseMatrix(matrix, {0, 3}, false);
      CHECK_FALSE(unsolvableSparseMatrix.isColumnPrimary(0));
      CHECK(unsolvableSparseMatrix.isColumnPrimary(1));
      CHECK(unsolvableSparseMatrix.isColumnPrimary(2));
      CHECK_FALSE(unsolvableSparseMatrix.isColumnPrimary(3));
      CHECK(unsolvableSparseMatrix.isColumnPrimary(4));
      CHECK(unsolvableSparseMatrix.isColumnPrimary(5));

      CHECK_FALSE(unsolvableSparseMatrix.isSolvableByAlgorithmX());

      // But if constructed by placing secondary columns at the end, the matrix becomes solvable
      SparseCoordinateMatrix solvableSparseMatrix(matrix, {0, 3});
      CHECK(solvableSparseMatrix.isColumnPrimary(0));
      CHECK(solvableSparseMatrix.isColumnPrimary(1));
      CHECK(solvableSparseMatrix.isColumnPrimary(2));
      CHECK(solvableSparseMatrix.isColumnPrimary(3));
      CHECK_FALSE(solvableSparseMatrix.isColumnPrimary(4));
      CHECK_FALSE(solvableSparseMatrix.isColumnPrimary(5));

      CHECK(solvableSparseMatrix.isSolvableByAlgorithmX());
    }

    SUBCASE("Empty primary columns") {
      {
        // Matrix with two all-zeroes columns at the end
        const std::vector<std::vector<bool>> invalidMatrix = {
            {1, 0, 1, 0, 0, 0, 0},
            {1, 1, 0, 1, 0, 0, 0},
            {0, 1, 1, 0, 0, 0, 0},
            {0, 0, 1, 1, 1, 0, 0},
            {0, 0, 0, 0, 0, 0, 0},
            {1, 1, 0, 0, 1, 0, 0},
        };

        SparseCoordinateMatrix unsolvableMatrix(invalidMatrix);
        // Now matrix is unsolvable, since a primary column is empty
        CHECK_FALSE(unsolvableMatrix.isSolvableByAlgorithmX());

        // But if the last empty columns are set to be secondary the matrix becomes solvable
        SparseCoordinateMatrix solvableMatrix(invalidMatrix, {5, 6});
        CHECK(solvableMatrix.isSolvableByAlgorithmX());
      }

      {
        // Matrix with an all-zeroes column in the middle
        const std::vector<std::vector<bool>> invalidMatrix = {
            {1, 0, 1, 0, 0, 0},
            {1, 1, 0, 0, 1, 0},
            {0, 1, 1, 0, 0, 0},
            {0, 0, 1, 0, 1, 1},
            {0, 0, 0, 0, 0, 0},
            {1, 1, 0, 0, 0, 1},
        };

        SparseCoordinateMatrix unsolvableMatrix(invalidMatrix);
        // Matrix is unsolvable, since a primary column is empty
        CHECK_FALSE(unsolvableMatrix.isSolvableByAlgorithmX());

        // Even if the empty column is set to be secondary the matrix remains unsolvable, as the secondary column is not
        // at the end
        SparseCoordinateMatrix solvableMatrix(invalidMatrix, {3}, false);
        CHECK_FALSE(solvableMatrix.isSolvableByAlgorithmX());
      }
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

    SparseCoordinateMatrix sparseMatrix{matrix, secondaryColumnIndices, false, matrixData};
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
