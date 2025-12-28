#pragma once

#include "OptionId.hpp"
#include "PuzzleIntrinsics.hpp"

#include <span>

template <PuzzleIntrinsics puzzle>
using OptionsSpan = std::array<std::span<const OptionId>, puzzle.rows * puzzle.columns * puzzle.digits.size()>;
