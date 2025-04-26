
#include "CellConstraint.hpp"
#include "ColumnConstraint.hpp"
#include "ConstraintInterface.hpp"
#include "RowConstraint.hpp"

#include <memory>

namespace ConstraintFactory {

template <PuzzleIntrinsics puzzleIntrinsics>
std::unique_ptr<ConstraintInterface<puzzleIntrinsics>> makeConstraint(ConstraintType constraintType) {
  switch (constraintType) {
  case ConstraintType::SUDOKU_CELL:
    return std::make_unique<CellConstraint<puzzleIntrinsics>>();
  case ConstraintType::SUDOKU_ROW:
    return std::make_unique<RowConstraint<puzzleIntrinsics>>();
  case ConstraintType::SUDOKU_COLUMN:
    return std::make_unique<ColumnConstraint<puzzleIntrinsics>>();

  default:
    return std::make_unique<CellConstraint<puzzleIntrinsics>>();
  }
}

} // namespace ConstraintFactory