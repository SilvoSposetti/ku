#include "ConstraintFactory.h"

#include "KingsMove.h"
#include "KingsMoveTorus.h"
#include "NegativeDiagonal.h"
#include "NegativeDiagonalEven.h"
#include "NegativeDiagonalOdd.h"
#include "PositiveDiagonal.h"
#include "PositiveDiagonalEven.h"
#include "PositiveDiagonalOdd.h"
#include "SudokuBox.h"
#include "SudokuCell.h"
#include "SudokuColumn.h"
#include "SudokuRow.h"

std::unique_ptr<AbstractConstraint> ConstraintFactory::makeConstraint(ConstraintType constraintType) {
  switch (constraintType) {
  case ConstraintType::SUDOKU_CELL:
    return std::make_unique<SudokuCell>();
  case ConstraintType::SUDOKU_BOX:
    return std::make_unique<SudokuBox>();
  case ConstraintType::SUDOKU_ROW:
    return std::make_unique<SudokuRow>();
  case ConstraintType::SUDOKU_COLUMN:
    return std::make_unique<SudokuColumn>();
  case ConstraintType::POSITIVE_DIAGONAL:
    return std::make_unique<PositiveDiagonal>();
  case ConstraintType::POSITIVE_DIAGONAL_EVEN:
    return std::make_unique<PositiveDiagonalEven>();
  case ConstraintType::POSITIVE_DIAGONAL_ODD:
    return std::make_unique<PositiveDiagonalOdd>();
  case ConstraintType::NEGATIVE_DIAGONAL:
    return std::make_unique<NegativeDiagonal>();
  case ConstraintType::NEGATIVE_DIAGONAL_EVEN:
    return std::make_unique<NegativeDiagonalEven>();
  case ConstraintType::NEGATIVE_DIAGONAL_ODD:
    return std::make_unique<NegativeDiagonalOdd>();
  case ConstraintType::KINGS_MOVE:
    return std::make_unique<KingsMove>();
  case ConstraintType::KINGS_MOVE_TORUS:
    return std::make_unique<KingsMoveTorus>();
  default:
    return std::make_unique<SudokuCell>();
  }
}
