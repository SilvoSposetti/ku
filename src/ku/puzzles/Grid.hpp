#pragma once

#include "Digits.hpp"
#include "PuzzleSpace.hpp"

/** The type for grids
 */
template <PuzzleSpace puzzleSpace>
using Grid = std::array<std::array<Digit, puzzleSpace.columnsCount>, puzzleSpace.rowsCount>;
