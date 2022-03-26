#pragma once

#include "../utilities/Utilities.h"
#include "Board.h"
#include "constraints/AbstractConstraint.h"

class Setter {
public:
  Setter() = delete;

  static std::unique_ptr<Board> generate(int32_t totalDigits,
                                         SymmetryType symmetryType,
                                         const std::vector<std::unique_ptr<AbstractConstraint>>& constraints);

private:
  static inline bool isOnMainDiagonal(int32_t rowIndex, int32_t columnIndex);

  static inline bool isOnCenter(int32_t rowIndex, int32_t columnIndex);

  static std::vector<std::vector<bool>> randomMask(int32_t totalDigits);

  static std::vector<std::vector<bool>> diagonalMirrorMask(int32_t totalDigits);

  static std::vector<std::vector<bool>> diagonalRotationMask(int32_t totalDigits);
};
