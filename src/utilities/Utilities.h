#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <random>
#include <chrono>
#include <atomic>

enum class Sudo : int8_t {
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

constexpr int8_t MAX_DIGIT = 9;
constexpr int8_t MIN_DIGIT = 1;
constexpr int8_t TOTAL_DIGITS = 81;
constexpr int8_t MAX_INDEX = 8;
constexpr int8_t MID_INDEX = 4;
constexpr int8_t MIN_INDEX = 0;
const std::vector<int8_t> INDICES = {0, 1, 2, 3, 4, 5, 6, 7, 8};
const std::vector<Sudo> SUDO_DIGITS = {Sudo::A, Sudo::B, Sudo::C, Sudo::D, Sudo::E, Sudo::F, Sudo::G, Sudo::H, Sudo::I};

enum class ConstraintType : uint64_t {
    NONE = 0,
    SUDOKU_CELL = 1 << 0, // Activated by default when creating new Sudoku, do not need to specify it
    SUDOKU_ROW = 1 << 1,
    SUDOKU_COLUMN = 1 << 2,
    SUDOKU_BOX = 1 << 3,
    MAIN_DIAGONAL = 1 << 4,
    ANTI_DIAGONAL = 1 << 5,
    KINGS_MOVE = 1 << 6,
};

inline ConstraintType operator|(ConstraintType a, ConstraintType b) {
    return static_cast<ConstraintType>(static_cast<uint64_t>(a) | static_cast<uint64_t>(b));
}

inline ConstraintType operator&(ConstraintType a, ConstraintType b) {
    return static_cast<ConstraintType>(static_cast<uint64_t>(a) & static_cast<uint64_t>(b));
}

static std::vector<std::vector<Sudo>> emptyField() {
    std::vector<std::vector<Sudo>> newField;
    for (int8_t i = 0; i <= MAX_INDEX; i++) {
        std::vector<Sudo> row(MAX_INDEX + 1, Sudo::NONE);
        newField.emplace_back(row);
    }
    return newField;
}

static std::vector<std::vector<bool>> fullGivenMask() {
    std::vector<std::vector<bool>> newMask;
    for (int8_t i = 0; i <= MAX_INDEX; i++) {
        std::vector<bool> row(MAX_INDEX + 1, true);
        newMask.emplace_back(row);
    }
    return newMask;
}

static std::vector<std::vector<bool>> emptyGivenMask() {
    std::vector<std::vector<bool>> newMask;
    for (int8_t i = 0; i <= MAX_INDEX; i++) {
        std::vector<bool> row(MAX_INDEX + 1, false);
        newMask.emplace_back(row);
    }
    return newMask;
}

enum class SymmetryType {
    RANDOM,
    ONE_DIAGONAL_MIRROR,
    ONE_DIAGONAL_ROTATION,
    TWO_DIAGONALS_MIRROR,
    TWO_DIAGONALS_ROTATION,
    ONE_AXIS_MIRROR,
    ONE_AXIS_ROTATION,
    TWO_AXES_MIRROR,
    TWO_AXES_ROTATION
};

static inline int8_t randomUniform(const int8_t min = MIN_INDEX, const int8_t max = MAX_INDEX) {
    std::random_device randomDevice;
    std::mt19937 generator(randomDevice());
    std::uniform_int_distribution<int8_t> uniformIntDistribution(min, max);
    return uniformIntDistribution(generator);
}

template<typename T>
static inline std::vector<T> randomShuffle(const std::vector<T>& input) {
    std::vector<T> vector = input;
    std::shuffle(vector.begin(), vector.end(), std::mt19937(std::random_device()()));
    return vector;
}

static inline int8_t clamp(const int8_t value, const int8_t min, const int8_t max) {
    return std::max(std::min(value, max), min);
}
