#pragma once

#include "Constraint.hpp"
#include "ConstraintMacros.hpp"
#include "SvgLine.hpp"

GENERATE_CONSTRAINT_TRAITS(ExactColumnConstraint, 1, 0);

template <PuzzleIntrinsics puzzle>
struct ExactColumnConstraint : public Constraint<ExactColumnConstraint<puzzle>, puzzle> {
public:
  constexpr ExactColumnConstraint()
      : Constraint<ExactColumnConstraint<puzzle>, puzzle>(
            ConstraintType::EXACT_COLUMN, "Column", "Columns contain all the digits exactly once.") {
    CONSTRAINT_CONCEPT_ASSERT(ExactColumnConstraint, puzzle);
  };

  static constexpr bool supportsPuzzle() {
    return puzzle.rows == puzzle.digits.size();
  }

  static constexpr Option<ConstraintTraits<ExactColumnConstraint<puzzle>>::primarySize>
  primaryOption([[maybe_unused]] uint32_t row, uint32_t column, uint32_t digit) {
    return {static_cast<OptionId>((column * static_cast<uint32_t>(puzzle.digits.size()) + (digit - 1)))};
  }

  static constexpr Option<ConstraintTraits<ExactColumnConstraint<puzzle>>::secondarySize>
  secondaryOption([[maybe_unused]] uint32_t row, [[maybe_unused]] uint32_t column, [[maybe_unused]] uint32_t digit) {
    return {};
  }

  virtual std::unique_ptr<SvgGroup>
  getSvgGroup(const DrawingOptionsTemplated<puzzle.getPuzzleSpace()>& options) const override {
    auto group = std::make_unique<SvgGroup>(this->getName(), std::nullopt, "black", options.thinLine);
    for (int32_t i = 0; i < puzzle.columns + 1; i++) {
      const double x = options.cellSize * i;
      group->add(std::make_unique<SvgLine>(x, 0.0, x, options.height));
    }
    return group;
  }
};
