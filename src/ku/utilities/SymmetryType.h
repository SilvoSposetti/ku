#pragma once

/** The types of symmetries that are supported for given digits.
 * The main diagonal is the "negative" one, going from top-left to bottom-right, while the other one is the "positive"
 * one, from bottom-left to top-right. The main axis is the vertical line spanning column 5, while the other is the
 * horizontal line spanning row 5.
 */
enum class SymmetryType {
  RANDOM, // No rule followed
  ONE_DIAGONAL_MIRROR, // Givens are mirrored on the main diagonal
  ONE_DIAGONAL_ROTATION, // Givens are rotated 180 degrees on the main diagonal
  TWO_DIAGONALS_MIRROR, // Givens are mirrored on both diagonals
  TWO_DIAGONALS_ROTATION, // Givens are rotated 90 degrees on both diagonals
  ONE_AXIS_MIRROR, // Givens are mirrored on the main axis
  ONE_AXIS_ROTATION, // Givens are rotated 180 degrees on the main axis
  TWO_AXES_MIRROR, // Givens are mirrored on both axes
  TWO_AXES_ROTATION, // Givens are rotated 90 degrees on both axes
};