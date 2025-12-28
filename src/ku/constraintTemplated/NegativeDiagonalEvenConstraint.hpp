#pragma once

#include "../drawing/SvgSquigglyLine.hpp"
#include "Constraint.hpp"
#include "ConstraintMacros.hpp"

GENERATE_CONSTRAINT_TRAITS(NegativeDiagonalEvenConstraint, 1, 0);

template <PuzzleIntrinsics puzzle>
struct NegativeDiagonalEvenConstraint : public Constraint<NegativeDiagonalEvenConstraint<puzzle>, puzzle> {
public:
  constexpr NegativeDiagonalEvenConstraint()
      : Constraint<NegativeDiagonalEvenConstraint<puzzle>, puzzle>(ConstraintType::NEGATIVE_DIAGONAL_EVEN,
                                                                   "Negative-Diagonal-Even",
                                                                   "The negative diagonal contains only even digits.") {
    CONSTRAINT_CONCEPT_ASSERT(NegativeDiagonalEvenConstraint, puzzle);
  };

  static constexpr bool supportsPuzzle() {
    return puzzle.isSquare() && puzzle.hasEvenDigits();
  }

  static constexpr Option<ConstraintTraits<NegativeDiagonalEvenConstraint<puzzle>>::primarySize>
  primaryOption(uint32_t row, uint32_t column, uint32_t digit) {
    if constexpr (puzzle.columns > 0) {
      if (puzzle.isOnNegativeDiagonal(row, column) && Digits::isEven(digit)) {
        return {static_cast<OptionId>((row))}; // Or column
      }
    }
    return {};
  }

  static constexpr Option<ConstraintTraits<NegativeDiagonalEvenConstraint<puzzle>>::secondarySize>
  secondaryOption([[maybe_unused]] uint32_t row, [[maybe_unused]] uint32_t column, [[maybe_unused]] uint32_t digit) {
    return {};
  }

  virtual std::unique_ptr<SvgGroup>
  getSvgGroup(const DrawingOptionsTemplated<puzzle.getPuzzleSpace()>& options) const override {
    auto group = std::make_unique<SvgGroup>(this->getName(), std::nullopt, "black", options.thinLine);
    group->add(std::make_unique<SvgSquigglyLine>(0, 0, options.width, options.height, options.cellSize / 10.0));
    return group;
  }
};
