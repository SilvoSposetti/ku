#pragma once

#include "../puzzles/Digits.hpp"
#include "../puzzles/Index.hpp"
#include "../puzzles/Option.hpp"
#include "../puzzles/PuzzleIntrinsics.hpp"

#include <concepts>

template <typename T, PuzzleIntrinsics intrinsics>
concept ConstraintConcept = std::default_initializable<T> && requires(Index row, Index column, Digit digit) {
  { T::primaryOption(row, column, digit) } -> std::same_as<std::optional<Option>>;
  { T::secondaryOption(row, column, digit) } -> std::same_as<std::optional<Option>>;
  { T::supportsPuzzle() } -> std::same_as<bool>;
};
