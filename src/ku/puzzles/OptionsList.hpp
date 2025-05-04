#pragma once

#include "PuzzleIntrinsics.hpp"

#include <vector>

template <PuzzleIntrinsics puzzle>
using OptionsList = std::array<std::vector<int32_t>, puzzle.rows * puzzle.columns * puzzle.digits.size()>;
