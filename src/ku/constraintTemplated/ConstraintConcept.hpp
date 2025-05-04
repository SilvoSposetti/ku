#pragma once

#include "../puzzles/ItemsList.hpp"
#include "../puzzles/PuzzleIntrinsics.hpp"

#include <concepts>

template <typename T, PuzzleIntrinsics intrinsics>
concept ConstraintConcept = std::default_initializable<T> && requires {
  { T::createPrimaryItems() } -> std::same_as<ItemsList<intrinsics>>;
  { T::createSecondaryItems() } -> std::same_as<ItemsList<intrinsics>>;
};
