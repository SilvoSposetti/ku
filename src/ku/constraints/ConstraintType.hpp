#pragma once

#include <cstdint>

enum class ConstraintType : uint64_t {
  NONE = 0,
  SUDOKU_CELL = 1 << 0,
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
  HYPER_SUDOKU = 1 << 16,
  // When adding a constraint, update below:
  // Constraint::getConstraintNameString()
};

inline ConstraintType operator|(ConstraintType a, ConstraintType b) {
  return static_cast<ConstraintType>(static_cast<uint64_t>(a) | static_cast<uint64_t>(b));
}

inline ConstraintType operator&(ConstraintType a, ConstraintType b) {
  return static_cast<ConstraintType>(static_cast<uint64_t>(a) & static_cast<uint64_t>(b));
}