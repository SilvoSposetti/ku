#pragma once

#include "PuzzleIntrinsics.hpp"

template <PuzzleIntrinsics puzzle>
using ItemsList = std::array<std::array<int32_t, 1>, puzzle.rows * puzzle.columns * puzzle.digits.size()>;
