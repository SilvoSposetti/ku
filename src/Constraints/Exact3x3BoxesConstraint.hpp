#pragma once

#include "SvgRect.hpp"
#include "Constraint.hpp"
#include "ConstraintMacros.hpp"

GENERATE_CONSTRAINT_TRAITS(Exact3x3BoxesConstraint, 1, 0);

template <PuzzleIntrinsics puzzle>
struct Exact3x3BoxesConstraint : public Constraint<Exact3x3BoxesConstraint<puzzle>, puzzle> {
public:
  constexpr Exact3x3BoxesConstraint()
      : Constraint<Exact3x3BoxesConstraint<puzzle>, puzzle>(
            ConstraintType::EXACT_3x3_BOXES, "Exact 3x3 Box", "3x3 boxes contain all the digits exactly once.") {
    CONSTRAINT_CONCEPT_ASSERT(Exact3x3BoxesConstraint, puzzle);
  };

  static constexpr bool supportsPuzzle() {
    return puzzle.rows % boxSize == 0 && puzzle.columns % boxSize == 0 && puzzle.digits.size() == 9;
  }

  static constexpr Option<ConstraintTraits<Exact3x3BoxesConstraint<puzzle>>::primarySize>
  primaryOption(uint32_t row, uint32_t column, uint32_t digit) {
    constexpr auto boxesVerticalCount = static_cast<uint32_t>(puzzle.columns) / boxSize;
    const auto boxId = (column / boxSize + boxesVerticalCount * (row / boxSize));
    return {static_cast<OptionId>(boxId * static_cast<uint32_t>(puzzle.digits.size()) + (digit - 1))};
  }

  static constexpr Option<ConstraintTraits<Exact3x3BoxesConstraint<puzzle>>::secondarySize>
  secondaryOption([[maybe_unused]] uint32_t row, [[maybe_unused]] uint32_t column, [[maybe_unused]] uint32_t digit) {
    return {};
  }

  virtual std::unique_ptr<SvgGroup>
  getSvgGroup(const DrawingOptionsTemplated<puzzle.getPuzzleSpace()>& options) const override {
    const auto boxesRows = puzzle.rows / boxSize;
    const auto boxesColumns = puzzle.columns / boxSize;
    const double boxLength = options.width / boxesColumns;

    auto group = std::make_unique<SvgGroup>(this->getName(), "transparent", "black", options.mediumLine);

    for (std::size_t i = 0; i < boxesRows; i++) {
      for (std::size_t j = 0; j < boxesColumns; j++) {
        group->add(std::make_unique<SvgRect>(j * boxLength, i * boxLength, boxLength, boxLength));
      }
    }
    return group;
  }

private:
  /// The size of the boxes
  static constexpr std::size_t boxSize = 3;
};
