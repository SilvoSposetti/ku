#pragma once

#include "SvgZigZagLine.hpp"
#include "Constraint.hpp"
#include "ConstraintMacros.hpp"

GENERATE_CONSTRAINT_TRAITS(PositiveDiagonalOddConstraint, 1, 0);

template <PuzzleIntrinsics puzzle>
struct PositiveDiagonalOddConstraint : public Constraint<PositiveDiagonalOddConstraint<puzzle>, puzzle> {
public:
  constexpr PositiveDiagonalOddConstraint()
      : Constraint<PositiveDiagonalOddConstraint<puzzle>, puzzle>(ConstraintType::POSITIVE_DIAGONAL_ODD,
                                                                  "Positive-Diagonal-Odd",
                                                                  "The positive diagonal contains only odd digits.") {
    CONSTRAINT_CONCEPT_ASSERT(PositiveDiagonalOddConstraint, puzzle);
  };

  static constexpr bool supportsPuzzle() {
    return puzzle.isSquare() && puzzle.hasOddDigits();
  }

  static constexpr Option<ConstraintTraits<PositiveDiagonalOddConstraint<puzzle>>::primarySize>
  primaryOption(uint32_t row, uint32_t column, uint32_t digit) {
    if constexpr (puzzle.columns > 0) {
      if (puzzle.isOnPositiveDiagonal(row, column) && Digits::isOdd(digit)) {
        return {static_cast<OptionId>((row))}; // Or column
      }
    }
    return {};
  }

  static constexpr Option<ConstraintTraits<PositiveDiagonalOddConstraint<puzzle>>::secondarySize>
  secondaryOption([[maybe_unused]] uint32_t row, [[maybe_unused]] uint32_t column, [[maybe_unused]] uint32_t digit) {
    return {};
  }

  virtual std::unique_ptr<SvgGroup>
  getSvgGroup(const DrawingOptionsTemplated<puzzle.getPuzzleSpace()>& options) const override {
    auto group = std::make_unique<SvgGroup>(this->getName(), std::nullopt, "black", options.thinLine);
    group->add(std::make_unique<SvgZigZagLine>(0, options.height, options.width, 0, options.cellSize / 10.0));
    return group;
  }
};
