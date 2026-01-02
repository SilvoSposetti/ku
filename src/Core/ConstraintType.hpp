#pragma once

#include <cstdint>

enum class ConstraintType : uint64_t {
  // TODO: rename these to pascal case
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
  KING_PATTERN = 1 << 10,
  KING_TORUS_PATTERN = 1 << 11,
  KNIGHT_PATTERN = 1 << 12,
  KNIGHT_TORUS_PATTERN = 1 << 13,
  DISJOINT_BOXES = 1 << 14,
  ASTERISK = 1 << 15,
  HYPER_SUDOKU = 1 << 16,
  //
  CELL = 1 << 0,
  EXACT_ROW = 1 << 1,
  EXACT_COLUMN = 1 << 2,
  EXACT_3x3_BOXES = 1 << 3,
  EXACT_POSITIVE_DIAGONAL = 1 << 4,
  EXACT_NEGATIVE_DIAGONAL = 1 << 7,
};

inline constexpr ConstraintType operator|(ConstraintType a, ConstraintType b) {
  return static_cast<ConstraintType>(static_cast<uint64_t>(a) | static_cast<uint64_t>(b));
}

inline constexpr ConstraintType operator&(ConstraintType a, ConstraintType b) {
  return static_cast<ConstraintType>(static_cast<uint64_t>(a) & static_cast<uint64_t>(b));
}
