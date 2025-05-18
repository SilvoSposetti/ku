
#include "CellConstraint.hpp"
#include "ConstraintInterface.hpp"
#include "Exact3x3BoxesConstraint.hpp"
#include "ExactColumnConstraint.hpp"
#include "ExactNegativeDiagonalConstraint.hpp"
#include "ExactPositiveDiagonalConstraint.hpp"
#include "ExactRowConstraint.hpp"
#include "HyperSudokuConstraint.hpp"

#include <memory>

namespace ConstraintFactory {

template <PuzzleIntrinsics intrinsics>
std::unique_ptr<ConstraintInterface<intrinsics>> makeConstraint(ConstraintType constraintType) {
  switch (constraintType) {
  case ConstraintType::CELL:
    return std::make_unique<CellConstraint<intrinsics>>();
  case ConstraintType::EXACT_ROW:
    if (ExactRowConstraint<intrinsics>::supportsPuzzle()) {
      return std::make_unique<ExactRowConstraint<intrinsics>>();
    } else {
      throw std::runtime_error("ExactRowConstraint does not support this puzzle");
    }
    break;
  case ConstraintType::EXACT_COLUMN:
    if (ExactColumnConstraint<intrinsics>::supportsPuzzle()) {
      return std::make_unique<ExactColumnConstraint<intrinsics>>();
    } else {
      throw std::runtime_error("ExactColumnConstraint does not support this puzzle");
    }
    break;
  case ConstraintType::EXACT_3x3_BOXES:
    if (Exact3x3BoxesConstraint<intrinsics>::supportsPuzzle()) {
      return std::make_unique<Exact3x3BoxesConstraint<intrinsics>>();
    } else {
      throw std::runtime_error("Exact3x3BoxesConstraint does not support this puzzle");
    }
    break;
  case ConstraintType::EXACT_POSITIVE_DIAGONAL:
    if (ExactPositiveDiagonalConstraint<intrinsics>::supportsPuzzle()) {
      return std::make_unique<ExactPositiveDiagonalConstraint<intrinsics>>();
    } else {
      throw std::runtime_error("ExactPositiveDiagonalConstraint does not support this puzzle");
    }
    break;
  case ConstraintType::EXACT_NEGATIVE_DIAGONAL:
    if (ExactNegativeDiagonalConstraint<intrinsics>::supportsPuzzle()) {
      return std::make_unique<ExactNegativeDiagonalConstraint<intrinsics>>();
    } else {
      throw std::runtime_error("ExactNegativeDiagonalConstraint does not support this puzzle");
    }
    break;
  case ConstraintType::HYPER_SUDOKU:
    if (HyperSudokuConstraint<intrinsics>::supportsPuzzle()) {
      return std::make_unique<HyperSudokuConstraint<intrinsics>>();
    } else {
      throw std::runtime_error("HyperSudokuConstraint does not support this puzzle");
    }
    break;
  default:
    break;
  }
  return std::make_unique<CellConstraint<intrinsics>>();
}

} // namespace ConstraintFactory