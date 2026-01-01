#pragma once

#include "Option.hpp"
#include "PuzzleIntrinsics.hpp"

template <PuzzleIntrinsics puzzle, std::size_t N>
using OptionsList = std::array<Option<N>, puzzle.rows * puzzle.columns * puzzle.digits.size()>;
