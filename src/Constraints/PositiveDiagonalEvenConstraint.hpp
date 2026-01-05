#pragma once

#include "Constraint.hpp"
#include "ConstraintMacros.hpp"
#include "SvgSquigglyLine.hpp"

GENERATE_CONSTRAINT_TRAITS(PositiveDiagonalEvenConstraint, 1, 0);

template <PuzzleIntrinsics puzzle>
struct PositiveDiagonalEvenConstraint : public Constraint<PositiveDiagonalEvenConstraint<puzzle>, puzzle> {
public:
  constexpr PositiveDiagonalEvenConstraint()
      : Constraint<PositiveDiagonalEvenConstraint<puzzle>, puzzle>(ConstraintType::POSITIVE_DIAGONAL_EVEN,
                                                                   "Positive-Diagonal-Even",
                                                                   "The positive diagonal contains only even digits.") {
    CONSTRAINT_CONCEPT_ASSERT(PositiveDiagonalEvenConstraint, puzzle);
  };

  static constexpr bool supportsPuzzle() {
    return puzzle.isSquare() && puzzle.hasEvenDigits();
  }

  static constexpr Option<ConstraintTraits<PositiveDiagonalEvenConstraint<puzzle>>::primarySize>
  primaryOption(uint32_t row, uint32_t column, uint32_t digit) {
    if constexpr (puzzle.columns > 0) {
      if (puzzle.isOnPositiveDiagonal(row, column) && Digits::isEven(digit)) {
        return {static_cast<OptionId>((row))}; // Or column
      }
    }
    return {};
  }

  static constexpr Option<ConstraintTraits<PositiveDiagonalEvenConstraint<puzzle>>::secondarySize>
  secondaryOption([[maybe_unused]] uint32_t row, [[maybe_unused]] uint32_t column, [[maybe_unused]] uint32_t digit) {
    return {};
  }

  virtual std::unique_ptr<SvgGroup> getSvgGroup(const DrawingOptions<puzzle.getPuzzleSpace()>& options) const override {
    auto group = std::make_unique<SvgGroup>(this->getName(), std::nullopt, "black", options.thinLine);
    group->add(std::make_unique<SvgSquigglyLine>(0, options.height, options.width, 0, options.cellSize / 10.0));
    return group;
  }
};
