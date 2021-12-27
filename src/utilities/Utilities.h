#pragma once

#include <iostream>
#include <vector>
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
const std::vector<int8_t> INDICES = {0, 1, 2, 3, 4, 5, 6, 7, 8};
const std::vector<Sudo> SUDO_DIGITS = {Sudo::A, Sudo::B, Sudo::C, Sudo::D, Sudo::E, Sudo::F, Sudo::G, Sudo::H, Sudo::I};

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

static std::vector<std::vector<Sudo>> emptyField() {
    std::vector<std::vector<Sudo>> newField;
    for (int8_t i = 0; i < MAX_DIGIT; i++) {
        std::vector<Sudo> row(MAX_DIGIT, Sudo::NONE);
        newField.emplace_back(row);
    }
    return newField;
}

static std::vector<std::vector<bool>> emptyGivenMask() {
    std::vector<std::vector<bool>> newMask;
    for (int8_t i = 0; i < MAX_DIGIT; i++) {
        std::vector<bool> row(MAX_DIGIT, true);
        newMask.emplace_back(row);
    }
    return newMask;
}

enum class SetterType {
    RANDOM = 0
};

static inline int8_t randomUniform(const int8_t min = 0, const int8_t max = 8) {
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
