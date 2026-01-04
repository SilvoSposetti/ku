#pragma once

#include "StringUtilities.hpp"
#include "Grid.hpp"
#include "PuzzleIntrinsics.hpp"

#include <string>
#include <vector>

namespace GridUtilities {

/** Constructs the grid visually as a list of lines
 * @tparam The puzzle space
 * @param The grid
 * @return A list of strings that represent the grid visually
 */
template <PuzzleSpace space>
std::vector<std::string> gridAsText(const Grid<space>& grid) {
  constexpr auto intrinsics = PuzzleIntrinsics<space>{};
  const auto maxDigitLength = !intrinsics.digits.empty() ? std::to_string(intrinsics.digits.back()).length() : 1;
  const auto invalidDigitCharacter = std::string{"◌"};

  const auto createTopOrBottomLine = [](const std::string& first,
                                        const std::string& blank,
                                        const std::string& last,
                                        std::size_t maxDigitLength,
                                        std::size_t rowDigitsCount) {
    auto element = std::string{};
    for (std::size_t i = 0; i < maxDigitLength; i++) {
      element += blank;
    }

    auto line = first + blank;
    for (std::size_t i = 0; i < rowDigitsCount; i++) {
      line += element + blank;
    }
    return line + last;
  };

  const auto createLine = [](const std::string& first,
                             const std::string& blank,
                             const std::string& last,
                             const std::vector<std::string>& elements) {
    auto line = first + blank;
    for (const auto& element : elements) {
      line += element + blank;
    }
    return line + last;
  };

  std::vector<std::string> lines;
  for (const auto& rowIndex : intrinsics.rowIndices) {
    if (rowIndex == intrinsics.rowIndices.front()) {
      lines.push_back(createTopOrBottomLine("┏", "━", "┓", maxDigitLength, space.columnsCount));
    }

    std::vector<std::string> digitStrings;
    std::ranges::transform(grid[rowIndex], std::back_inserter(digitStrings), [&](const auto& digit) {
      auto string = std::string{};
      if (Digits::isValid(digit)) {
        string = std::to_string(digit);
        string = StringUtilities::padLeft(string, ' ', maxDigitLength);

      } else {
        string = StringUtilities::padLeft(string, ' ', maxDigitLength - 1);
        string += invalidDigitCharacter;
      }

      return StringUtilities::padLeft(string, ' ', maxDigitLength);
    });

    lines.push_back(createLine("┃", " ", "┃", digitStrings));

    if (rowIndex == intrinsics.rowIndices.back()) {
      lines.push_back(createTopOrBottomLine("┗", "━", "┛", maxDigitLength, space.columnsCount));
    }
  }
  return lines;
}

} // namespace GridUtilities
