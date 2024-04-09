#include "ConstraintFactory.h"

#include "AntiKing.h"
#include "AntiKingTorus.h"
#include "AntiKnight.h"
#include "AntiKnightTorus.h"
#include "Asterisk.h"
#include "DisjointBoxes.h"
#include "HyperSudoku.h"
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
  std::unique_ptr<AbstractConstraint> constraint;
  switch (constraintType) {
  case ConstraintType::SUDOKU_CELL:
    constraint = std::make_unique<SudokuCell>();
    break;
  case ConstraintType::SUDOKU_BOX:
    constraint = std::make_unique<SudokuBox>();
    break;
  case ConstraintType::SUDOKU_ROW:
    constraint = std::make_unique<SudokuRow>();
    break;
  case ConstraintType::SUDOKU_COLUMN:
    constraint = std::make_unique<SudokuColumn>();
    break;
  case ConstraintType::POSITIVE_DIAGONAL:
    constraint = std::make_unique<PositiveDiagonal>();
    break;
  case ConstraintType::POSITIVE_DIAGONAL_EVEN:
    constraint = std::make_unique<PositiveDiagonalEven>();
    break;
  case ConstraintType::POSITIVE_DIAGONAL_ODD:
    constraint = std::make_unique<PositiveDiagonalOdd>();
    break;
  case ConstraintType::NEGATIVE_DIAGONAL:
    constraint = std::make_unique<NegativeDiagonal>();
    break;
  case ConstraintType::NEGATIVE_DIAGONAL_EVEN:
    constraint = std::make_unique<NegativeDiagonalEven>();
    break;
  case ConstraintType::NEGATIVE_DIAGONAL_ODD:
    constraint = std::make_unique<NegativeDiagonalOdd>();
    break;
  case ConstraintType::ANTI_KING:
    constraint = std::make_unique<AntiKing>();
    break;
  case ConstraintType::ANTI_KING_TORUS:
    constraint = std::make_unique<AntiKingTorus>();
    break;
  case ConstraintType::ANTI_KNIGHT:
    constraint = std::make_unique<AntiKnight>();
    break;
  case ConstraintType::ANTI_KNIGHT_TORUS:
    constraint = std::make_unique<AntiKnightTorus>();
    break;
  case ConstraintType::DISJOINT_BOXES:
    constraint = std::make_unique<DisjointBoxes>();
    break;
  case ConstraintType::ASTERISK:
    constraint = std::make_unique<Asterisk>();
    break;
  case ConstraintType::HYPER_SUDOKU:
    constraint = std::make_unique<HyperSudoku>();
    break;
  default:
    constraint = std::make_unique<SudokuCell>();
    break;
  }
  constraint->initialize();
  return constraint;
}
