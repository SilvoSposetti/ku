#pragma once

#include "../drawing/SvgLine.hpp"
#include "Constraint.hpp"
#include "ConstraintMacros.hpp"

GENERATE_CONSTRAINT_TRAITS(ExactRowConstraint, 1, 0);

template <PuzzleIntrinsics puzzle>
struct ExactRowConstraint : public Constraint<ExactRowConstraint<puzzle>, puzzle> {
public:
  constexpr ExactRowConstraint()
      : Constraint<ExactRowConstraint<puzzle>, puzzle>(
            ConstraintType::EXACT_ROW, "Row", "Rows contain all the digits exactly once.") {
    CONSTRAINT_CONCEPT_ASSERT(ExactRowConstraint, puzzle);
  };

  static constexpr bool supportsPuzzle() {
    return puzzle.columns == puzzle.digits.size();
  }

  static constexpr Option<ConstraintTraits<ExactRowConstraint<puzzle>>::primarySize>
  primaryOption(uint32_t row, [[maybe_unused]] uint32_t column, uint32_t digit) {
    return {static_cast<OptionId>(row * static_cast<uint32_t>(puzzle.digits.size()) + (digit - 1))};
  }

  static constexpr Option<ConstraintTraits<ExactRowConstraint<puzzle>>::secondarySize>
  secondaryOption([[maybe_unused]] uint32_t row, [[maybe_unused]] uint32_t column, [[maybe_unused]] uint32_t digit) {
    return {};
  }

  virtual std::unique_ptr<SvgGroup>
  getSvgGroup(const DrawingOptionsTemplated<puzzle.getPuzzleSpace()>& options) const override {
    auto group = std::make_unique<SvgGroup>(this->getName(), std::nullopt, "black", options.thinLine);
    for (int32_t i = 0; i < puzzle.rows + 1; i++) {
      const double y = options.cellSize * i;
      group->add(std::make_unique<SvgLine>(0, y, options.width, y));
    }
    return group;
  }
};