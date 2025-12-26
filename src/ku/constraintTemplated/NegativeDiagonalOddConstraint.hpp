#pragma once

#include "../drawing/SvgZigZagLine.hpp"
#include "Constraint.hpp"
#include "ConstraintMacros.hpp"

GENERATE_CONSTRAINT_TRAITS(NegativeDiagonalOddConstraint, 1, 0);

template <PuzzleIntrinsics puzzle>
struct NegativeDiagonalOddConstraint : public Constraint<NegativeDiagonalOddConstraint<puzzle>, puzzle> {
public:
  constexpr NegativeDiagonalOddConstraint()
      : Constraint<NegativeDiagonalOddConstraint<puzzle>, puzzle>(ConstraintType::NEGATIVE_DIAGONAL_ODD,
                                                                  "Negative-Diagonal-Odd",
                                                                  "The negative diagonal contains only odd digits.") {
    CONSTRAINT_CONCEPT_ASSERT(NegativeDiagonalOddConstraint, puzzle);
  };

  static constexpr bool supportsPuzzle() {
    return puzzle.isSquare() && puzzle.hasOddDigits();
  }

  static constexpr Option<ConstraintTraits<NegativeDiagonalOddConstraint<puzzle>>::primarySize>
  primaryOption(uint32_t row, uint32_t column, uint32_t digit) {
    if constexpr (puzzle.columns > 0) {
      if (puzzle.isOnNegativeDiagonal(row, column) && Digits::isOdd(digit)) {
        return {static_cast<OptionId>((row))}; // Or column
      }
    }
    return {};
  }

  static constexpr Option<ConstraintTraits<NegativeDiagonalOddConstraint<puzzle>>::secondarySize>
  secondaryOption([[maybe_unused]] uint32_t row, [[maybe_unused]] uint32_t column, [[maybe_unused]] uint32_t digit) {
    return {};
  }

  virtual std::unique_ptr<SvgGroup>
  getSvgGroup(const DrawingOptionsTemplated<puzzle.getPuzzleSpace()>& options) const override {
    auto group = std::make_unique<SvgGroup>(this->getName(), std::nullopt, "black", options.thinLine);
    group->add(std::make_unique<SvgZigZagLine>(0, 0, options.width, options.height, options.cellSize / 10.0));
    return group;
  }
};