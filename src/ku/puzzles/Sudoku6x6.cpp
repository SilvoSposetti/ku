#include "Sudoku6x6.hpp"

#include <print>

void Sudoku6x6::printGrid() const {
  const auto maxRowIndex = rowIndices.back();
  const auto boxSizeHorizontal = 3;
  const auto boxSizeVertical = 2;
  for (const auto& i : columnIndices) {
    if (i == 0) {
      std::println("┏━━━━━━━┯━━━━━━━┓");
    }
    for (const auto j : rowIndices) {
      if (j == 0) {
        std::print("┃ ");
      }
      if (grid[i][j] != Digits::invalidDigit) {
        std::print("{}", grid[i][j]);
      } else {
        std::print("◌");
      }
      if (j < maxRowIndex) {
        if (j % boxSizeHorizontal == (boxSizeHorizontal - 1))
          std::print(" │ ");
        else
          std::print(" ");
      } else if (j == maxRowIndex) {

        std::println(" ┃");
      }
    }
    if (i % boxSizeVertical == (boxSizeVertical - 1) && i < maxRowIndex) {
      std::println("┠───────┼───────┨");

    } else if (i == maxRowIndex) {
      std::println("┗━━━━━━━┷━━━━━━━┛");
    }
  }
};
