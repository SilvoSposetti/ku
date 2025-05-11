#pragma once

#include "../puzzles/Option.hpp"
#include "../puzzles/PuzzleIntrinsics.hpp"

#include <concepts>

template <typename T, PuzzleIntrinsics intrinsics>
concept ConstraintConcept = std::default_initializable<T> && requires(uint32_t row, uint32_t column, uint32_t digit) {
  { T::primaryOption(row, column, digit) } -> std::same_as<Option>;
  { T::secondaryOption(row, column, digit) } -> std::same_as<Option>;
  { T::supportsPuzzle() } -> std::same_as<bool>;
};
