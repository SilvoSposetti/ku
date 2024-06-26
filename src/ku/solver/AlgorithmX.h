#pragma once

#include "DataStructure.h"

#include <cstdint>
#include <optional>
#include <unordered_set>
#include <vector>

/** Namespace that interfaces with Algorithm X, a recursive, nondeterministic, depth-first, backtracking algorithm.
 * It is used to find solutions of Exact Cover problems described by sparse binary matrices.
 */
namespace AlgorithmX {

/** Solves the Exact Cover problem described by the matrix and retrieves all solutions.
 * @param dataStructure The sparse matrix representing the Exact Cover problem
 * @param seed The seed for the random number generator. Uses a random seed if not available.
 * @return Returns potentially zero, one or many sets of options (matrix-rows) indices that solve the matrix's problem
 */
std::vector<std::unordered_set<int32_t>> findAllSolutions(const DataStructure& dataStructure,
                                                          const std::optional<int32_t>& seed);

/** Solves the Exact Cover problem described by the matrix and retrieves the first solution found.
 * @param dataStructure The sparse matrix representing the Exact Cover problem
 * @param seed The seed for the random number generator. Uses a random seed if not available.
 * @return Returns potentially zero, one or many sets of options (matrix-rows) indices that solve the matrix's problem
 */
std::unordered_set<int32_t> findOneSolution(const DataStructure& dataStructure, const std::optional<int32_t>& seed);

/** Computes whether the Exact Cover problem has exactly one solution. Has a potential early exit with respect to run()
 * since it returns false as soon as it finds 2 solutions.
 * @param dataStructure The sparse matrix representing the Exact Cover problem
 * @param seed The seed for the random number generator. Uses a random seed if not available.
 * @return Whether exactly one solution exists to the problem defined by the matrix.
 */
bool hasUniqueSolution(const DataStructure& dataStructure, const std::optional<int32_t>& seed);

}; // namespace AlgorithmX
