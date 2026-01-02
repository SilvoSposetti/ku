#pragma once

#include "SvgLine.hpp"
#include "Constraint.hpp"
#include "ConstraintMacros.hpp"

GENERATE_CONSTRAINT_TRAITS(ExactNegativeDiagonalConstraint, 1, 0);

template <PuzzleIntrinsics puzzle>
struct ExactNegativeDiagonalConstraint : public Constraint<ExactNegativeDiagonalConstraint<puzzle>, puzzle> {
public:
  constexpr ExactNegativeDiagonalConstraint()
      : Constraint<ExactNegativeDiagonalConstraint<puzzle>, puzzle>(
            ConstraintType::EXACT_NEGATIVE_DIAGONAL,
            "Exact-Negative-Diagonal",
            "The negative diagonal contains all digits exactly once.") {
    CONSTRAINT_CONCEPT_ASSERT(ExactNegativeDiagonalConstraint, puzzle);
  };

  static constexpr bool supportsPuzzle() {
    return puzzle.isSquare() && puzzle.rows == puzzle.digits.size();
  }

  static constexpr Option<ConstraintTraits<ExactNegativeDiagonalConstraint<puzzle>>::primarySize>
  primaryOption(uint32_t row, uint32_t column, uint32_t digit) {
    if constexpr (puzzle.columns > 0) {
      if (puzzle.isOnNegativeDiagonal(row, column)) {
        return {static_cast<OptionId>((digit - 1))};
      }
    }
    return {};
  }

  static constexpr Option<ConstraintTraits<ExactNegativeDiagonalConstraint<puzzle>>::secondarySize>
  secondaryOption([[maybe_unused]] uint32_t row, [[maybe_unused]] uint32_t column, [[maybe_unused]] uint32_t digit) {
    return {};
  }

  virtual std::unique_ptr<SvgGroup>
  getSvgGroup(const DrawingOptionsTemplated<puzzle.getPuzzleSpace()>& options) const override {
    auto group = std::make_unique<SvgGroup>(this->getName(), std::nullopt, "black", options.thinLine);
    group->add(std::make_unique<SvgLine>(0, 0, options.width, options.height));
    return group;
  }
};
