#pragma once

#include "Constraint.hpp"
#include "utilities/NeighborPatternUtilities.hpp"

/** Base class for pattern constraints.
 * @tparam Derived The derived class, necessary for the parent's CRTP pattern.
 * @tparam puzzle The puzzle
 * @tparam N The size of the pattern
 * @tparam pattern The pattern of offsets
 * @tparam isTorus Whether the pattern is allowed to create cell connections across puzzle boundaries in the form of
 * toroidal wrapping.
 */
template <typename Derived,
          PuzzleIntrinsics puzzle,
          size_t N,
          std::array<std::pair<int32_t, int32_t>, N> pattern,
          bool isTorus>
struct PatternConstraint : public Constraint<Derived, puzzle> {

  constexpr PatternConstraint(ConstraintType type, std::string_view name, std::string_view description)
      : Constraint<Derived, puzzle>(type, name, description) {}

  static constexpr bool supportsPuzzle() {
    return puzzle.digits.size() >= pattern.size() + 1;
  }

  static constexpr Option<ConstraintTraits<Derived>::primarySize> primaryOption(uint32_t, uint32_t, uint32_t) {
    return {};
  }

  static constexpr Option<ConstraintTraits<Derived>::secondarySize>
  secondaryOption(uint32_t row, uint32_t col, uint32_t digit) {
    return NeighborPatternUtilities::computePatternOption<puzzle, N, ConstraintTraits<Derived>::secondarySize, isTorus>(
        row, col, digit, pattern);
  }
};