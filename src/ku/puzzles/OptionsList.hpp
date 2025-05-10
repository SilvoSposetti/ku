#pragma once

#include "Option.hpp"
#include "PuzzleIntrinsics.hpp"

template <PuzzleIntrinsics puzzle>
using OptionsList = std::array<Option, puzzle.rows * puzzle.columns * puzzle.digits.size()>;
