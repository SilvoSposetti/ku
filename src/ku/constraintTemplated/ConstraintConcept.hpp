#pragma once

#include "../puzzles/OptionsList.hpp"
#include "../puzzles/PuzzleIntrinsics.hpp"

#include <concepts>

template <typename T, PuzzleIntrinsics intrinsics>
concept ConstraintConcept = std::default_initializable<T> && requires {
  { T::createPrimaryItems() } -> std::same_as<OptionsList<intrinsics>>;
  { T::createSecondaryItems() } -> std::same_as<OptionsList<intrinsics>>;
};
