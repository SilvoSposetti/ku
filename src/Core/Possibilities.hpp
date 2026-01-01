#pragma once

#include "Cell.hpp"
#include "PuzzleSpace.hpp"

template <PuzzleSpace space>
using Possibilities = std::array<Cell, space.rowsCount * space.columnsCount * space.digitsCount>;
