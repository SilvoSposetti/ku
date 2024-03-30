#pragma once
#include <cstdint>
#include <vector>

namespace ConstraintUtilities {

/** Generates all index pairs that identify the cells of the positive diagonal
 * @return The set of index pairs of the positive diagonal
 */
std::vector<std::pair<int32_t, int32_t>> getAllPositiveDiagonalIndices();

/** Computes whether a cell is on the positive diagonal
 * @param rowIndex The row index of the cell
 * @param columnIndex The column index of the cell
 * @return Whether the cell is on the positive diagonal
 */
bool isOnPositiveDiagonal(int32_t rowIndex, int32_t columnIndex);

/** Generates all index pairs that identify the cells of the negative diagonal
 * @return The set of index pairs of the negative diagonal
 */
std::vector<std::pair<int32_t, int32_t>> getAllNegativeDiagonalIndices();

/** Computes whether a cell is on the neagative diagonal
 * @param rowIndex The row index of the cell
 * @param columnIndex The column index of the cell
 * @return Whether the cell is on the negative diagonal
 */
bool isOnNegativeDiagonal(int32_t i, int32_t j);

} // namespace ConstraintUtilities