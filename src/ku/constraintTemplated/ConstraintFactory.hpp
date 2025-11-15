
#include "AntiKingConstraint.hpp"
#include "AntiKingTorusConstraint.hpp"
#include "AntiKnightTorusConstraint.hpp"
#include "AsteriskSudokuConstraint.hpp"
#include "CellConstraint.hpp"
#include "ConstraintInterface.hpp"
#include "Disjoint3x3BoxesSudokuConstraint.hpp"
#include "Exact3x3BoxesConstraint.hpp"
#include "ExactColumnConstraint.hpp"
#include "ExactNegativeDiagonalConstraint.hpp"
#include "ExactPositiveDiagonalConstraint.hpp"
#include "ExactRowConstraint.hpp"
#include "HyperSudokuConstraint.hpp"
#include "NegativeDiagonalEvenConstraint.hpp"
#include "NegativeDiagonalOddConstraint.hpp"
#include "PositiveDiagonalEvenConstraint.hpp"
#include "PositiveDiagonalOddConstraint.hpp"

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
  case ConstraintType::POSITIVE_DIAGONAL_EVEN:
    if (PositiveDiagonalEvenConstraint<intrinsics>::supportsPuzzle()) {
      return std::make_unique<PositiveDiagonalEvenConstraint<intrinsics>>();
    } else {
      throw std::runtime_error("PositiveDiagonalEvenConstraint does not support this puzzle");
    }
    break;
  case ConstraintType::POSITIVE_DIAGONAL_ODD:
    if (PositiveDiagonalOddConstraint<intrinsics>::supportsPuzzle()) {
      return std::make_unique<PositiveDiagonalOddConstraint<intrinsics>>();
    } else {
      throw std::runtime_error("PositiveDiagonalOddConstraint does not support this puzzle");
    }
    break;
  case ConstraintType::EXACT_NEGATIVE_DIAGONAL:
    if (ExactNegativeDiagonalConstraint<intrinsics>::supportsPuzzle()) {
      return std::make_unique<ExactNegativeDiagonalConstraint<intrinsics>>();
    } else {
      throw std::runtime_error("ExactNegativeDiagonalConstraint does not support this puzzle");
    }
  case ConstraintType::NEGATIVE_DIAGONAL_EVEN:
    if (NegativeDiagonalEvenConstraint<intrinsics>::supportsPuzzle()) {
      return std::make_unique<NegativeDiagonalEvenConstraint<intrinsics>>();
    } else {
      throw std::runtime_error("NegativeDiagonalEvenConstraint does not support this puzzle");
    }
    break;
  case ConstraintType::NEGATIVE_DIAGONAL_ODD:
    if (NegativeDiagonalOddConstraint<intrinsics>::supportsPuzzle()) {
      return std::make_unique<NegativeDiagonalOddConstraint<intrinsics>>();
    } else {
      throw std::runtime_error("NegativeDiagonalOddConstraint does not support this puzzle");
    }
    break;
  case ConstraintType::HYPER_SUDOKU:
    if (HyperSudokuConstraint<intrinsics>::supportsPuzzle()) {
      return std::make_unique<HyperSudokuConstraint<intrinsics>>();
    } else {
      throw std::runtime_error("HyperSudokuConstraint does not support this puzzle");
    }
    break;
  case ConstraintType::DISJOINT_BOXES:
    if (Disjoint3x3BoxesSudokuConstraint<intrinsics>::supportsPuzzle()) {
      return std::make_unique<Disjoint3x3BoxesSudokuConstraint<intrinsics>>();
    } else {
      throw std::runtime_error("Disjoint3x3BoxesSudokuConstraint does not support this puzzle");
    }
    break;
  case ConstraintType::ASTERISK:
    if (AsteriskSudokuConstraint<intrinsics>::supportsPuzzle()) {
      return std::make_unique<AsteriskSudokuConstraint<intrinsics>>();
    } else {
      throw std::runtime_error("AsteriskSudokuConstraint does not support this puzzle");
    }
    break;
  case ConstraintType::ANTI_KING_TORUS:
    if (AntiKingTorusConstraint<intrinsics>::supportsPuzzle()) {
      return std::make_unique<AntiKingTorusConstraint<intrinsics>>();
    } else {
      throw std::runtime_error("AntiKingTorusConstraint does not support this puzzle");
    }
    break;
  case ConstraintType::ANTI_KNIGHT_TORUS:
    if (AntiKnightTorusConstraint<intrinsics>::supportsPuzzle()) {
      return std::make_unique<AntiKnightTorusConstraint<intrinsics>>();
    } else {
      throw std::runtime_error("AntiKnightTorusConstraint does not support this puzzle");
    }
    break;
  case ConstraintType::ANTI_KING:
    if (AntiKingConstraint<intrinsics>::supportsPuzzle()) {
      return std::make_unique<AntiKingConstraint<intrinsics>>();
    } else {
      throw std::runtime_error("AntiKingConstraint does not support this puzzle");
    }
    break;
  default:
    break;
  }
  return std::make_unique<CellConstraint<intrinsics>>();
}

} // namespace ConstraintFactory