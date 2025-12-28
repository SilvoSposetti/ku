#pragma once
#include "../../puzzles/Option.hpp"
#include "../../puzzles/PuzzleIntrinsics.hpp"

#include <ranges>
/** Utility functions for patterns of cell neighbors
 */
namespace NeighborPatternUtilities {

/** Concept that defines the size of a pattern.
 * It can't be empty or have an odd size.
 */
template <std::size_t size>
concept SizeNonZeroAndEvenConcept = (size % 2 == 0) && (size != 0);

/** Computes whether a pattern is valid.
 * A pattern is valid if it is non-empty and has an even amount of digits (restricted by concept), if it contains no
 * duplicates, and if for any offset provided its radial counterpart also appears in the pattern.
 * Note that because of the above restrictions offset {0, 0}, the central cell of the patten, cannot appear.
 * @tparam patternSize The concept-restricted size of the pattern
 * @param pattern The pattern of cell offsets.
 */
template <std::size_t patternSize>
  requires SizeNonZeroAndEvenConcept<patternSize>
constexpr bool isValidPattern(const std::array<std::pair<int32_t, int32_t>, patternSize>& pattern) {
  for (std::size_t i = 0; i < patternSize; i++) {
    const auto& [offsetX, offsetY] = pattern[i];
    // Check if any equal offset after i is a duplicate
    if (std::any_of(pattern.begin() + i + 1, pattern.end(), [&](const auto& element) {
          return element.first == offsetX && element.second == offsetY;
        })) {
      return false;
    }
    // Check radial counterpart exists
    if (!std::ranges::any_of(
            pattern, [&](const auto& element) { return element.first == -offsetX && element.second == -offsetY; })) {
      return false;
    }
  }
  return true;
};

/** Given an offset, it determines if this offset happens before (backward) or after (forward) the central cell in
 * row-major order.
 * @param offsetRow The offset of the rows, positive value means the offset points below the central cell.
 * @param offsetColumn The offset of the columns, positive value means the offset points to the right of the central
 * cell.
 * @return Whether the offset points forward in row-major order with respect to the central cell.
 */
constexpr bool isOffsetForwardInRowMajorOrder(int32_t offsetRow, int32_t offsetColumn) {
  return offsetRow > 0 || (offsetRow == 0 && offsetColumn > 0);
}

/** Counts how many unique pattern connections can be made with a given pattern up to and not including cell (i, j) in
 * row-major order.
 * @tparam puzzle The puzzle
 * @tparam isTorus Whether cells can see each other through toroidal wrapping.
 * @tparam K the size of the pattern
 * @param pattern The pattern of offsets
 * @param i The row index of the cell
 * @param j The column index of the cell
 * @return The amount of unique pattern connections.
 */
template <PuzzleIntrinsics puzzle, bool isTorus, std::size_t K>
  requires SizeNonZeroAndEvenConcept<K>
constexpr std::size_t
countUniquePatternConnectionsUpTo(const std::array<std::pair<int32_t, int32_t>, K>& pattern, int32_t i, int32_t j) {
  constexpr auto rowsCount = std::int32_t(puzzle.rows);
  constexpr auto columnsCount = std::int32_t(puzzle.columns);

  if (!puzzle.isValidRowIndex(i) || !puzzle.isValidColumnIndex(j)) {
    return 0;
  }

  const auto cellsBeforeThis = std::size_t(i) * columnsCount + std::size_t(j);

  if constexpr (isTorus) {
    // For the case where the pattern is allowed to wrap around the grid, just count how many cells there are before
    // the current one and multiply the result by how many offsets are in the pattern plus another implied (0, 0)
    // offset such that the cell itself is also counted. return cellsBeforeThis * (pattern.size() + 1);
    return cellsBeforeThis * pattern.size() / 2;
  }

  // If (i, j) is the very first cell, no connection could have been made yet
  if (i == 0 && j == 0) {
    return 0;
  }

  // Find predecessor
  const auto previousCell = puzzle.previousCellInRowMajorOrder(i, j);
  if (!previousCell.has_value()) {
    return 0;
  }
  const auto previousI = int32_t{previousCell.value().first};
  const auto previousJ = int32_t{previousCell.value().second};

  std::size_t total = 0;
  for (const auto& [offsetRow, offsetColumn] : pattern) {

    // Only count when source is strictly earlier in row-major order than destination
    if (!isOffsetForwardInRowMajorOrder(offsetRow, offsetColumn)) {
      continue;
    }

    // Valid source rows r so that r+offsetRow in [0, rowsCount-1]
    int32_t rowMin = std::max(0, -offsetRow);
    int32_t rowMax = std::min(rowsCount - 1, rowsCount - 1 - offsetRow);
    if (rowMin > rowMax) {
      continue;
    }

    // Valid source columns c so that c+offsetColumn in [0, columnsCount-1]
    int32_t columnMin = std::max(0, -offsetColumn);
    int32_t columnMax = std::min(columnsCount - 1, columnsCount - 1 - offsetColumn);
    if (columnMin > columnMax) {
      continue;
    }

    // Rows fully before i
    int fullRowsEnd = std::min(previousI - 1, rowMax);
    std::size_t fullRows = 0;
    if (rowMin <= fullRowsEnd) {
      fullRows = std::size_t(fullRowsEnd - rowMin + 1);
    }

    std::size_t columnsPerFullRows = std::size_t(columnMax - columnMin + 1);
    total += fullRows * columnsPerFullRows;

    // For row where i is on: only columns <= j
    if (previousI >= rowMin && previousI <= rowMax) {
      int columnsMaxAllowed = std::min(columnMax, previousJ);
      if (columnMin <= columnsMaxAllowed) {
        total += std::size_t(columnsMaxAllowed - columnMin + 1);
      }
    }
  }
  return total;
}

/** Computes the sorted forward pattern from another pattern. Sorting is done clockwise, with the first element pointing
 * towards the right, if two offsets have the same angle (e.g. (1, 1) and (2, 2)) then the shortest one is sorted before
 * the longest one
 * @tparam patternSize The size of the pattern
 * @param pattern The pattern of offsets
 * @return The forward pattern, of half size with respect to the original pattern.
 */
template <std::size_t patternSize>
  requires SizeNonZeroAndEvenConcept<patternSize>
constexpr std::array<std::pair<int32_t, int32_t>, patternSize / 2>
sortedForwardPattern(const std::array<std::pair<int32_t, int32_t>, patternSize>& pattern) {
  std::array<std::pair<int32_t, int32_t>, patternSize / 2> forwardResult{};
  std::ranges::copy_if(pattern, forwardResult.begin(), [&](const std::pair<int32_t, int32_t>& offset) {
    return isOffsetForwardInRowMajorOrder(offset.first, offset.second);
  });

  const auto distanceSquared = [&](const auto& offset) -> std::size_t {
    return offset.second * offset.second + offset.first * offset.first;
  };

  const auto crossProduct = [&](const auto& offsetA, const auto& offsetB) {
    return int64_t(offsetA.second) * int64_t(offsetB.first) - int64_t(offsetA.first) * int64_t(offsetB.second);
  };

  std::ranges::sort(forwardResult, [&](const auto& offsetA, const auto& offsetB) {
    const auto cross = crossProduct(offsetA, offsetB);
    if (cross != 0) {
      return cross > 0; // Cross product positive means a is clockwise before b
    }

    // If they have the same direction (cross product is zero) -> shorter first
    return distanceSquared(offsetA) < distanceSquared(offsetB);
  });

  return forwardResult;
}

/** Helper to find a neighbor in the 2D grid starting from a cell and moving through an offset
 * @tparam puzzle The puzzle
 * @tparam isTorus Whether offset is allowed to wrap around the 2D grid
 * @param rowIndex The row index
 * @param columnIndex The column index
 * @param rowOffset The row offset
 * @param columnOffset The column offset
 * @return If available, the neighbor
 */
template <PuzzleIntrinsics puzzle, bool isTorus>
constexpr std::optional<std::pair<int32_t, int32_t>>
computeNeighbor(int32_t rowIndex, int32_t columnIndex, int32_t rowOffset, int32_t columnOffset) {
  if constexpr (isTorus) {
    return puzzle.computeNeighborTorus(rowIndex, columnIndex, rowOffset, columnOffset);
  } else {
    return puzzle.computeNeighbor(rowIndex, columnIndex, rowOffset, columnOffset);
  }
}

/** Computes the torus option for a pattern.
 * @tparam puzzle The puzzle
 * @tparam forwardPatternSize The size of the array of unique offsets
 * @tparam optionSize The size of the requested Option to be computed
 * @param rowIndex The row index in the puzzle for which the option is to be computed
 * @param columnIndex The column index in the puzzle for which the option is to be computed
 * @param digit The digit for which the option is to be computed
 * @param forwardPattern The unique forward offsets that define half of the pattern
 * @return The option for the given inputs
 */
template <PuzzleIntrinsics puzzle, std::size_t forwardPatternSize, std::size_t optionSize>
constexpr Option<optionSize>
computePatternOptionTorus(uint32_t rowIndex,
                          uint32_t columnIndex,
                          uint32_t digit,
                          const std::array<std::pair<int32_t, int32_t>, forwardPatternSize>& forwardPattern) {

  auto option = Option<optionSize>();
  // Set this cell 'patternSize' times. That is, the times that other cells will find the current one in their
  // pattern
  const auto cellId = puzzle.computeCellId(rowIndex, columnIndex);
  if (cellId.has_value()) {
    for (std::size_t i = 0; i < forwardPatternSize; i++) {
      option.pushBack(static_cast<OptionId>(cellId.value() * forwardPatternSize + i) * puzzle.digits.size() +
                      (digit - 1));
    }
  }

  // Set the 'patternSize' amount of cells constructed using the pattern from the current one
  std::size_t id = 0;
  for (const auto& offset : forwardPattern) {
    const auto location = puzzle.computeNeighborTorus(rowIndex, columnIndex, offset.first, offset.second);
    if (location.has_value()) {
      const auto neighborId = puzzle.computeCellId(location.value().first, location.value().second);
      if (neighborId.has_value()) {
        option.pushBack(static_cast<OptionId>((neighborId.value() * forwardPatternSize + id++) * puzzle.digits.size() +
                                              (digit - 1)));
      }
    }
  }
  return option;
}

/** Computes the non-torus option for a pattern.
 * @tparam puzzle The puzzle
 * @tparam patternSize The size of the array of unique offsets
 * @tparam optionSize The size of the requested Option to be computed
 * @param rowIndex The row index in the puzzle for which the option is to be computed
 * @param columnIndex The column index in the puzzle for which the option is to be computed
 * @param digit The digit for which the option is to be computed
 * @param pattern The unique offsets that define the pattern
 * @param forwardPattern The unique forward offsets that define half of the pattern
 * @return The option for the given inputs
 */
template <PuzzleIntrinsics puzzle, std::size_t patternSize, std::size_t optionSize>
  requires SizeNonZeroAndEvenConcept<patternSize>
constexpr Option<optionSize>
computePatternOptionNonTorus(uint32_t rowIndex,
                             uint32_t columnIndex,
                             uint32_t digit,
                             const std::array<std::pair<int32_t, int32_t>, patternSize>& pattern,
                             const std::array<std::pair<int32_t, int32_t>, patternSize / 2>& forwardPattern) {
  auto option = Option<optionSize>();

  const auto currentCellBaseId = countUniquePatternConnectionsUpTo<puzzle, false>(pattern, rowIndex, columnIndex);
  // Looking forward
  auto forwardId = std::size_t{0};
  for (const auto& [rowOffset, columnOffset] : forwardPattern) {
    auto forwardLocationOptional = computeNeighbor<puzzle, false>(rowIndex, columnIndex, rowOffset, columnOffset);
    // Cell (rowIndex, columnIndex) will have as many "forward" pattern connections as there are cells
    // that are reachable from it using the forward pattern.
    if (forwardLocationOptional.has_value()) {
      // Add an option for every forward cell that's reachable, going forward with the IDs
      option.pushBack(static_cast<OptionId>((currentCellBaseId + forwardId) * puzzle.digits.size() + (digit - 1)));
      forwardId++;
    }
  }

  // Looking backward
  for (const auto& [rowOffset, columnOffset] : forwardPattern) {
    auto backwardLocationOptional = computeNeighbor<puzzle, false>(rowIndex, columnIndex, -rowOffset, -columnOffset);
    // Cell (rowIndex, columnIndex) will have as many "backward" pattern connections as there are cells
    // that are reachable from it using the forward pattern backwards.
    if (backwardLocationOptional.has_value()) {
      const auto& backwardLocation = backwardLocationOptional.value();
      // To compute the relative IDs of the cells in the "backward" pattern, look at the next cell from
      // backwardLocation and compute its base ID. From there subtract how many cells are reachable until the
      // current cell is found.
      const auto nextBackwardLocationOptional =
          puzzle.nextCellInRowMajorOrder(backwardLocation.first, backwardLocation.second);
      if (nextBackwardLocationOptional.has_value()) {
        const auto& nextBackwardLocation = nextBackwardLocationOptional.value();
        const auto nextBaseId = countUniquePatternConnectionsUpTo<puzzle, false>(
            pattern, nextBackwardLocation.first, nextBackwardLocation.second);
        auto backwardId = std::size_t{0};
        // Look in reverse through the forward pattern starting from the backwad cell, count how many connections
        // can be made, and stop when the current cell is found. The "negative" id found is the value that needs to
        // be removed from nextBaseId.
        for (const auto& [rowO, columnO] : std::views::reverse(forwardPattern)) {
          const auto otherNeighborOptional =
              computeNeighbor<puzzle, false>(backwardLocation.first, backwardLocation.second, rowO, columnO);
          if (otherNeighborOptional.has_value()) {
            backwardId++;
            const auto& otherNeighbor = otherNeighborOptional.value();
            if (otherNeighbor ==
                std::make_pair(static_cast<std::int32_t>(rowIndex), static_cast<std::int32_t>(columnIndex))) {
              break;
            }
          }
        }
        // Add an option for the current backward cell that's reachable, going backward with the IDs from the base
        // of the next cell.
        option.pushBack(static_cast<OptionId>((nextBaseId - backwardId) * puzzle.digits.size() + (digit - 1)));
      }
    }
  }
  return option;
}

/** Helper to compute an option for a pattern.
 * @tparam puzzle The puzzle
 * @tparam patternSize The size of the array of unique offsets
 * @tparam optionSize The size of the requested Option to be computed
 * @tparam isTorus Whether the pattern is allowed to wrap around the 2D grid
 * @param rowIndex The row index in the puzzle for which the option is to be computed
 * @param columnIndex The column index in the puzzle for which the option is to be computed
 * @param digit The digit for which the option is to be computed
 * @param pattern The unique offsets that define the whole pattern
 * @return The option for the given inputs
 */
template <PuzzleIntrinsics puzzle, std::size_t patternSize, std::size_t optionSize, bool isTorus>
  requires SizeNonZeroAndEvenConcept<patternSize>
constexpr Option<optionSize> computePatternOption(uint32_t rowIndex,
                                                  uint32_t columnIndex,
                                                  uint32_t digit,
                                                  const std::array<std::pair<int32_t, int32_t>, patternSize>& pattern) {

  auto option = Option<optionSize>();
  const auto forwardPattern = sortedForwardPattern(pattern);

  if constexpr (isTorus) {
    option =
        computePatternOptionTorus<puzzle, patternSize / 2, optionSize>(rowIndex, columnIndex, digit, forwardPattern);
  } else {
    option = computePatternOptionNonTorus<puzzle, patternSize, optionSize>(
        rowIndex, columnIndex, digit, pattern, forwardPattern);
  }
  // Sort the option before returning it
  option.sort();
  return option;
}

} // namespace NeighborPatternUtilities