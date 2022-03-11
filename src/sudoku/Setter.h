#pragma once

#include "Board.h"
#include "../utilities/Utilities.h"
#include "constraints/AbstractConstraint.h"
#include <cstdint>

class Setter {
public:
    Setter() = delete;

    static std::unique_ptr<Board> generate(int8_t totalDigits, 
                                           SymmetryType symmetryType,
                                           const std::vector<std::unique_ptr<AbstractConstraint>>& constraints);

private:


    static inline bool isOnMainDiagonal(int8_t rowIndex,
                                        int8_t columnIndex);

    static inline bool isOnCenter(int8_t rowIndex,
                                  int8_t columnIndex);

    static std::vector<std::vector<bool>> randomMask(int8_t totalDigits);

    static std::vector<std::vector<bool>> diagonalMirrorMask(int8_t totalDigits);

    static std::vector<std::vector<bool>> diagonalRotationMask(int8_t totalDigits);
};



