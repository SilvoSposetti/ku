#pragma once

#include "Constraint.hpp"
#include "NeighborPatternUtilities.hpp"
#include "SvgLine.hpp"

#include <cmath>
#include <set>

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

  virtual std::unique_ptr<SvgGroup> getSvgGroup(const DrawingOptions<puzzle.getPuzzleSpace()>& options) const override {
    // Create set of all unique cell connections
    std::set<std::pair<std::pair<int32_t, int32_t>, std::pair<int32_t, int32_t>>> connections;
    for (const auto& rowIndex : puzzle.rowIndices) {
      for (const auto& columnIndex : puzzle.columnIndices) {
        const auto currentCell = std::pair<int32_t, int32_t>{rowIndex, columnIndex};
        for (const auto& [rowOffset, columnOffset] : pattern) {
          auto neighbor = std::optional<std::pair<int32_t, int32_t>>{};
          if constexpr (isTorus) {
            neighbor = std::make_pair(static_cast<int32_t>(rowIndex) + static_cast<int32_t>(rowOffset),
                                      static_cast<int32_t>(columnIndex) + static_cast<int32_t>(columnOffset));
          } else {
            neighbor = puzzle.computeNeighbor(rowIndex, columnIndex, rowOffset, columnOffset);
          }
          if (neighbor.has_value()) {
            auto pair = std::array<std::pair<int32_t, int32_t>, 2>{currentCell, neighbor.value()};
            std::ranges::sort(pair);
            connections.insert(std::make_pair(pair[0], pair[1]));
          }
        }
      }
    }

    // Create connections group
    const double distanceFromCenter = (1 - .618) * options.cellSize;
    auto group = std::make_unique<SvgGroup>(this->getName(), std::nullopt, "black", options.thinLine);
    const auto halfCellSize = options.cellSize / 2;
    for (const auto& connection : connections) {
      const auto start = std::make_pair(halfCellSize + options.cellSize * connection.first.first,
                                        halfCellSize + options.cellSize * connection.first.second);
      const auto end = std::make_pair(halfCellSize + options.cellSize * connection.second.first,
                                      halfCellSize + options.cellSize * connection.second.second);
      const auto startEnd = std::make_pair(end.first - start.first, end.second - start.second);
      const auto norm = std::sqrt(startEnd.first * startEnd.first + startEnd.second * startEnd.second);
      const auto direction = std::make_pair(startEnd.first / norm, startEnd.second / norm);

      group->add(std::make_unique<SvgLine>(start.first + direction.first * distanceFromCenter,
                                           start.second + direction.second * distanceFromCenter,
                                           end.first - direction.first * distanceFromCenter,
                                           end.second - direction.second * distanceFromCenter));
    }
    return group;
  }
};
