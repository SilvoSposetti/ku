#pragma once

#include "Constraint.hpp"
#include "ConstraintMacros.hpp"
#include "SvgCircle.hpp"

GENERATE_CONSTRAINT_TRAITS(Disjoint3x3BoxesSudokuConstraint, 1, 0);

template <PuzzleIntrinsics puzzle>
struct Disjoint3x3BoxesSudokuConstraint : public Constraint<Disjoint3x3BoxesSudokuConstraint<puzzle>, puzzle> {
public:
  constexpr Disjoint3x3BoxesSudokuConstraint()
      : Constraint<Disjoint3x3BoxesSudokuConstraint<puzzle>, puzzle>(
            ConstraintType::DISJOINT_BOXES,
            "Disjoint-Boxes",
            "Cells in the same relative position within all 3x3 boxes contain all the digits exactly once.") {
    CONSTRAINT_CONCEPT_ASSERT(Disjoint3x3BoxesSudokuConstraint, puzzle);
  };

  static constexpr bool supportsPuzzle() {
    return puzzle.isSudoku();
  }

  static constexpr Option<ConstraintTraits<Disjoint3x3BoxesSudokuConstraint<puzzle>>::primarySize>
  primaryOption(uint32_t row, uint32_t column, uint32_t digit) {
    constexpr auto boxesVerticalCount = std::size_t{3};
    const auto idInBox = (column % boxSize + boxesVerticalCount * (row % boxSize));
    return {static_cast<OptionId>(idInBox * static_cast<uint32_t>(puzzle.digits.size()) + (digit - 1))};
  }

  static constexpr Option<ConstraintTraits<Disjoint3x3BoxesSudokuConstraint<puzzle>>::secondarySize>
  secondaryOption([[maybe_unused]] uint32_t row, [[maybe_unused]] uint32_t column, [[maybe_unused]] uint32_t digit) {
    return {};
  }

  virtual std::unique_ptr<SvgGroup>
  getSvgGroup(const DrawingOptionsTemplated<puzzle.getPuzzleSpace()>& options) const override {
    auto group = std::make_unique<SvgGroup>(this->getName(), "transparent", "black", options.thinLine);
    const double circleRadius = options.cellSize / 15.0;
    const double squareOffset = (options.cellSize / 2.0) * 0.75;
    for (const auto i : puzzle.rowIndices) {
      for (const auto j : puzzle.columnIndices) {
        const double positionX =
            static_cast<double>(options.cellSize * (i + .5)) + squareOffset * (static_cast<double>(i % boxSize) - 1);
        const double positionY =
            static_cast<double>(options.cellSize * (j + .5)) + squareOffset * (static_cast<double>(j % boxSize) - 1);
        group->add(std::make_unique<SvgCircle>(positionX, positionY, circleRadius));
      }
    }
    return group;
  }

private:
  /// The box size
  static constexpr std::size_t boxSize = 3;
};
