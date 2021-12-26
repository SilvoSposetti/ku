#pragma once

#include <iostream>

constexpr u_int8_t MAX_DIGITS = 8;

enum class Sudo : uint8_t {
    NONE = 0,
    A = 1,
    B = 2,
    C = 3,
    D = 4,
    E = 5,
    F = 6,
    G = 7,
    H = 8,
    I = 9
};

enum class ConstraintType : uint64_t {
    SUDOKU_BOX = 1 << 0,
    SUDOKU_ROW = 1 << 1,
    SUDOKU_COLUMN = 1 << 2,
    MAIN_DIAGONAL = 1 << 3,
    ANTI_DIAGONAL = 1 << 4,
    KINGS_MOVE = 1 << 5,
};

inline ConstraintType operator|(ConstraintType a, ConstraintType b) {
    return static_cast<ConstraintType>(static_cast<uint64_t>(a) | static_cast<uint64_t>(b));
}

inline ConstraintType operator&(ConstraintType a, ConstraintType b) {
    return static_cast<ConstraintType>(static_cast<uint64_t>(a) & static_cast<uint64_t>(b));
}

enum class SetterType {
    RANDOM = 0
};

