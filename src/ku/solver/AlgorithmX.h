#pragma once

#include "SparseCoordinateMatrix.h"

#include <string>
#include <unordered_set>
#include <vector>

/** Namespace that interfaces with Algorithm X. It is used to find solutions of Exact Cover problems described by
 * sparse matrices.
 */
namespace AlgorithmX {

/** Solves the Exact Cover problem described by the matrix and retrieves none, one or many solutions.
 * @param sparseMatrix The sparse matrix representing the Exact Cover problem
 * @return Returns potentially zero, one or many sets of options (matrix-rows) indices that solve the matrix's problem
 */
std::vector<std::unordered_set<int32_t>> run(const SparseCoordinateMatrix& sparseMatrix);

/** Computes whether the Exact Cover problem has exactly one solution. Has a potential early exit with respect to run()
 * since it returns false as soon as it finds 2 solutions.
 * @param sparseMatrix The sparse matrix representing the Exact Cover problem
 * @return Whether exactly one solution exists to the problem defined by the matrix.
 */
bool hasUniqueSolution(const SparseCoordinateMatrix& sparseMatrix);

/** Logs to standard output the internal data structure used. Useful for debugging.
 * @param sparseMatrix The sparse matrix representing the Exact Cover problem
 */
void printDataStructure(const SparseCoordinateMatrix& sparseMatrix);

}; // namespace AlgorithmX
