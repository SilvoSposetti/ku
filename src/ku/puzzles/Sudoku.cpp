#include "Sudoku.hpp"

#include <cstdio>
#include <string>

void Sudoku::printGrid() const {
  const auto maxRowIndex = rowIndices.back();
  const auto boxSize = 3;
  for (const auto& i : columnIndices) {
    if (i == 0) {
      std::puts("┏━━━━━━━┯━━━━━━━┯━━━━━━━┓");
    }
    std::string line;
    for (const auto& j : rowIndices) {
      if (j == 0) {
        line += "┃ ";
      }
      if (grid[i][j] != Digits::invalidDigit) {
        line += std::to_string(grid[i][j]);
      } else {
        line += "◌";
      }
      if (j < maxRowIndex) {
        if (j % boxSize == (boxSize - 1))
          line += " │ ";
        else
          line += " ";
      } else if (j == maxRowIndex) {

        line += " ┃";
      }
    }
    std::puts(line.c_str());

    if (i % boxSize == (boxSize - 1) && i < maxRowIndex) {
      std::puts("┠───────┼───────┼───────┨");

    } else if (i == maxRowIndex) {
      std::puts("┗━━━━━━━┷━━━━━━━┷━━━━━━━┛");
    }
  }
};