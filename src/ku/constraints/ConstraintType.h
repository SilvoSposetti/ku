#pragma once
#include "../utilities/Utilities.h"

#include <unordered_set>

enum class ConstraintType : uint64_t {
  NONE = 0,
  SUDOKU_CELL = 1 << 0, // Activated by default when creating new Sudoku, do not need to specify it
  SUDOKU_ROW = 1 << 1,
  SUDOKU_COLUMN = 1 << 2,
  SUDOKU_BOX = 1 << 3,
  POSITIVE_DIAGONAL = 1 << 4,
  POSITIVE_DIAGONAL_EVEN = 1 << 5,
  POSITIVE_DIAGONAL_ODD = 1 << 6,
  NEGATIVE_DIAGONAL = 1 << 7,
  NEGATIVE_DIAGONAL_EVEN = 1 << 8,
  NEGATIVE_DIAGONAL_ODD = 1 << 9,
  ANTI_KING = 1 << 10,
  ANTI_KING_TORUS = 1 << 11,
  ANTI_KNIGHT = 1 << 12,
  ANTI_KNIGHT_TORUS = 1 << 13,
  DISJOINT_BOXES = 1 << 14,
  ASTERISK = 1 << 15,
  // When adding a constraint, update:
  // 1. Constraint::ALL_CONSTRAINTS set
  // 2. Constraint::getConstraintName()
};

inline ConstraintType operator|(ConstraintType a, ConstraintType b) {
  return static_cast<ConstraintType>(static_cast<uint64_t>(a) | static_cast<uint64_t>(b));
}

inline ConstraintType operator&(ConstraintType a, ConstraintType b) {
  return static_cast<ConstraintType>(static_cast<uint64_t>(a) & static_cast<uint64_t>(b));
}

namespace Constraint {
const std::unordered_set<ConstraintType> ALL_CONSTRAINTS_SET = {
    ConstraintType::SUDOKU_CELL,
    ConstraintType::SUDOKU_ROW,
    ConstraintType::SUDOKU_COLUMN,
    ConstraintType::SUDOKU_BOX,
    ConstraintType::POSITIVE_DIAGONAL,
    ConstraintType::POSITIVE_DIAGONAL_EVEN,
    ConstraintType::POSITIVE_DIAGONAL_ODD,
    ConstraintType::NEGATIVE_DIAGONAL,
    ConstraintType::NEGATIVE_DIAGONAL_EVEN,
    ConstraintType::NEGATIVE_DIAGONAL_ODD,
    ConstraintType::ANTI_KING,
    ConstraintType::ANTI_KING_TORUS,
    ConstraintType::ANTI_KNIGHT,
    ConstraintType::ANTI_KNIGHT_TORUS,
    ConstraintType::DISJOINT_BOXES,
    ConstraintType::ASTERISK,
};

inline ConstraintType getSudokuConstraints() {
  return ConstraintType::SUDOKU_CELL | ConstraintType::SUDOKU_ROW | ConstraintType::SUDOKU_COLUMN |
         ConstraintType::SUDOKU_BOX;
}

static inline std::string getConstraintNameString(ConstraintType type) {
  switch (type) {
  case ConstraintType::SUDOKU_CELL:
    return "Sudoku-Cell";
  case ConstraintType::SUDOKU_ROW:
    return "Sudoku-Row";
  case ConstraintType::SUDOKU_COLUMN:
    return "Sudoku-Column";
  case ConstraintType::SUDOKU_BOX:
    return "Sudoku-Box";
  case ConstraintType::POSITIVE_DIAGONAL:
    return "Positive-Diagonal";
  case ConstraintType::POSITIVE_DIAGONAL_EVEN:
    return "Positive-Diagonal-Even";
  case ConstraintType::POSITIVE_DIAGONAL_ODD:
    return "Positive-Diagonal-Odd";
  case ConstraintType::NEGATIVE_DIAGONAL:
    return "Negative-Diagonal";
  case ConstraintType::NEGATIVE_DIAGONAL_EVEN:
    return "Negative-Diagonal-Even";
  case ConstraintType::NEGATIVE_DIAGONAL_ODD:
    return "Negative-Diagonal-Odd";
  case ConstraintType::ANTI_KING:
    return "Anti-King";
  case ConstraintType::ANTI_KING_TORUS:
    return "Anti-King-Torus";
  case ConstraintType::ANTI_KNIGHT:
    return "Anti-Knight";
  case ConstraintType::ANTI_KNIGHT_TORUS:
    return "Anti-Knight-Torus";
  case ConstraintType::DISJOINT_BOXES:
    return "Disjoint-Boxes";
  case ConstraintType::ASTERISK:
    return "Asterisk";
  default:
    return "Unknown Constraint";
  }
}
} // namespace Constraint
