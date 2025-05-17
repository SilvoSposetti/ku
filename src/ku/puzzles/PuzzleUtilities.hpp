#pragma once
#include "PuzzleIntrinsics.hpp"

namespace PuzzleUtilities {

template <PuzzleIntrinsics intrinsics>
constexpr bool isOnPositiveDiagonal(uint32_t rowIndex, uint32_t columnIndex) {
  if constexpr (intrinsics.rows > 0 && intrinsics.columns > 0) {
    if constexpr (intrinsics.rows == intrinsics.columns) {
      if (rowIndex < intrinsics.rows && columnIndex < intrinsics.columns) {
        return rowIndex + columnIndex == (intrinsics.rowIndices.back()); // or also puzzle.columnIndices.back()
      }
    }
  }
  return false;
}

template <PuzzleIntrinsics intrinsics>
constexpr bool isOnNegativeDiagonal(uint32_t rowIndex, uint32_t columnIndex) {
  if constexpr (intrinsics.rows > 0 && intrinsics.columns > 0) {
    if constexpr (intrinsics.rows == intrinsics.columns) {
      if (rowIndex < intrinsics.rows && columnIndex < intrinsics.columns) {
        return rowIndex == columnIndex;
      }
    }
  }
  return false;
}

} // namespace PuzzleUtilities