#pragma once
#include "../../puzzles/Option.hpp"
#include "../../puzzles/PuzzleIntrinsics.hpp"

namespace NeighborPatternUtilities {

/** @tparam puzzle The puzzle
 * @tparam patternSize The size of the array of unique offsets
 * @tparam optionSize The size of the requested Option to be computed
 * @param rowIndex The row index in the puzzle for which the option is to be computed
 * @param columnIndex The column index in the puzzle for which the option is to be computed
 * @param pattern The unique offsets that define the pattern
 * @return The Option for the given inputs
 */
template <PuzzleIntrinsics puzzle, std::size_t patternSize, std::size_t optionSize>
constexpr Option<optionSize>
computeNeighborPatternTorusOption(uint32_t rowIndex,
                                  uint32_t columnIndex,
                                  uint32_t digit,
                                  std::array<std::pair<int32_t, int32_t>, patternSize> pattern) {

  auto option = Option<optionSize>();

  // Set this cell 'patternSize' times. That is, the times that other cells will find the current one in their pattern
  const auto cellId = puzzle.computeCellId(rowIndex, columnIndex);
  if (cellId.has_value()) {
    for (std::size_t i = 0; i < patternSize; i++) {
      option.pushBack(static_cast<OptionId>(cellId.value() * patternSize + i) * puzzle.digits.size() + (digit - 1));
    }
  }

  // Set the 'patternSize' amount of cells constructed using the pattern from the current one
  std::size_t id = 0;
  for (const auto& offset : pattern) {
    const auto location = puzzle.computeNeighborTorus(rowIndex, columnIndex, offset.first, offset.second);
    if (location.has_value()) {
      const auto neighborId = puzzle.computeCellId(location.value().first, location.value().second);
      if (neighborId.has_value()) {
        option.pushBack(
            static_cast<OptionId>((neighborId.value() * patternSize + id++) * puzzle.digits.size() + (digit - 1)));
      }
    }
  }

  // Sort the option before returning it
  std::ranges::sort(option);
  return option;
}

} // namespace NeighborPatternUtilities