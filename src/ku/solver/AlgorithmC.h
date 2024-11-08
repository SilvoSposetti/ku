#pragma once

#include "DancingCellsStructure.h"

#include <cstdint>
#include <optional>
#include <unordered_set>

/** Namespace that interfaces with Algorithm C, a recursive, nondeterministic, depth-first, backtracking algorithm.
 * It is used to find solutions of "Exact Covering with Colors" (XCC) problems.
 */
namespace AlgorithmC {

/** Solves the Exact Covering with Colors problem described by the Dancing Cells data structure and retrieves all
 * possible solutions.
 * @param dataStructure The data structure representing Exact Covering with Colors problem.
 * @param seed The seed for the random number generator. Uses a random seed if not available.
 * @return Returns potentially zero, one or many sets of option indices that solve the XCC problem. The solutions
 * retrieved are in the order that they are found.
 */
std::vector<std::unordered_set<int32_t>> findAllSolutions(DancingCellsStructure& dataStructure,
                                                          const std::optional<int32_t>& seed);

/** Solves the Exact Covering with Colors problem described by the matrix and retrieves the first solution found.
 * @param dataStructure The data structure representing Exact Covering with Colors problem.
 * @param seed The seed for the random number generator. Uses a random seed if not available.
 * @return If there's at least one, a set of option indices that solve the XCC problem. This solution is not guaranteed
 * to be unique. Returns an empty optional if there are no solutions.
 */
std::optional<std::unordered_set<int32_t>> findOneSolution(DancingCellsStructure& dataStructure,
                                                           const std::optional<int32_t>& seed);

/** Computes whether the Exact Covering with Colors problem has exactly one solution. In the case of multiple solutions
 * being present, it has a potential early exit since it can return soon as it finds the second solution. Not needing to
 * explore all the space to know if it is unique.
 * @param dataStructure The data structure representing Exact Covering with Colors problem.
 * @param seed The seed for the random number generator. Uses a random seed if not available.
 * @return If exactly one solution exists, then that solution. Otherwise an empty optional.
 */
std::optional<std::unordered_set<int32_t>> hasUniqueSolution(DancingCellsStructure& dataStructure,
                                                             const std::optional<int32_t>& seed);

}; // namespace AlgorithmC
