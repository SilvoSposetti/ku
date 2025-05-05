
#include "CellConstraint.hpp"
#include "ColumnConstraint.hpp"
#include "ConstraintInterface.hpp"
#include "RowConstraint.hpp"

#include <memory>

namespace ConstraintFactory {

template <PuzzleIntrinsics intrinsics>
std::unique_ptr<ConstraintInterface<intrinsics>> makeConstraint(ConstraintType constraintType) {
  switch (constraintType) {
  case ConstraintType::CELL:
    return std::make_unique<CellConstraint<intrinsics>>();
  case ConstraintType::EXACT_ROW:
    if (RowConstraint<intrinsics>::supportsPuzzle()) {
      return std::make_unique<RowConstraint<intrinsics>>();
    } else {
      throw std::runtime_error("RowConstraint does not support this puzzle");
    }
    break;
  case ConstraintType::EXACT_COLUMN:
    if (ColumnConstraint<intrinsics>::supportsPuzzle()) {
      return std::make_unique<ColumnConstraint<intrinsics>>();
    } else {
      throw std::runtime_error("ColumnConstraint does not support this puzzle");
    }
    break;
  default:
    break;
  }
  return std::make_unique<CellConstraint<intrinsics>>();
}

} // namespace ConstraintFactory