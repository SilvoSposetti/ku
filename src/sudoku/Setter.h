#pragma once

#include "Board.h"
#include "../utilities/Utilities.h"
#include "constraints/AbstractConstraint.h"

class Setter {
public:
    Setter() = delete;

    static std::unique_ptr<Board> generate(const int8_t totalDigits, const SymmetryType symmetryType,
                                           const std::vector<std::unique_ptr<AbstractConstraint>>& constraints);

private:
    static void checkGivenMask(const std::vector<std::vector<bool>>& mask, const int8_t totalDigits);

    static inline bool isOnMainDiagonal(const int8_t rowIndex,
                                        const int8_t columnIndex);

    static inline bool isOnCenter(const int8_t rowIndex,
                                  const int8_t columnIndex);

    static std::vector<std::vector<bool>> randomMask(const int8_t totalDigits);

    static std::vector<std::vector<bool>> diagonalMirrorMask(const int8_t totalDigits);

    static std::vector<std::vector<bool>> diagonalRotationMask(const int8_t totalDigits);
};



