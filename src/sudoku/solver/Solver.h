#pragma once

#include "../../utilities/Utilities.h"
#include "../constraints/AbstractConstraint.h"

enum class SolverType {
    BruteForce, // Standard naive brute-force approach
    DLX // Transforms the sudoku instance into an exact cover instance and solves it with Algorithm X using Dancing Links
};

class Solver {
public:
    static std::vector<std::vector<Sudo>>
    createBoard(const std::vector<std::unique_ptr<AbstractConstraint>>& constraints, const SolverType solverType);


private:

    static bool randomBruteForceRecursive(int8_t rowIndex,
                                          int8_t columnIndex,
                                          std::vector<std::vector<Sudo>>& board,
                                          const std::vector<std::vector<bool>>& givenMask,
                                          const std::vector<std::unique_ptr<AbstractConstraint>>& constraints);

    static bool randomDLX(int8_t rowIndex,
                          int8_t columnIndex,
                          std::vector<std::vector<Sudo>>& board,
                          const std::vector<std::vector<bool>>& givenMask,
                          const std::vector<std::unique_ptr<AbstractConstraint>>& constraints);
};

