#pragma once

#include "../../utilities/Utilities.h"
#include "../constraints/AbstractConstraint.h"
#include "Node.h"
#include <cstdint>

enum class SolverType {
    BruteForce, // Standard naive brute-force approach
    DLX // Transforms the sudoku instance into an exact cover instance and solves it with Algorithm X using Dancing Links
};

class Solver {
public:
    static std::vector<std::vector<Sudo>>
    createBoard(const std::vector<std::unique_ptr<AbstractConstraint>>& constraints, SolverType solverType);


private:

    static bool randomBruteForceRecursive(int8_t rowIndex,
                                          int8_t columnIndex,
                                          std::vector<std::vector<Sudo>>& board,
                                          const std::vector<std::vector<bool>>& givenMask,
                                          const std::vector<std::unique_ptr<AbstractConstraint>>& constraints);

    static bool randomDlx(std::vector<std::vector<Sudo>>& board,
                          const std::vector<std::vector<bool>>& givenMask,
                          const std::vector<std::unique_ptr<AbstractConstraint>>& constraints);

    static std::shared_ptr<Node> createDancingLinksMatrix(const std::vector<std::vector<int32_t>>& matrix,
                                                          const std::vector<std::unique_ptr<AbstractConstraint>>& constraints);

    static std::vector<std::vector<std::shared_ptr<Node>>> searchDlx(const std::shared_ptr<Node>& root);

    static void searchDlxRecursive(const std::shared_ptr<Node>& root,
                                   int32_t depth,
                                   int32_t& solutionsLeftToSearchFor,
                                   std::vector<std::shared_ptr<Node>> solutionHolder,
                                   std::vector<std::vector<std::shared_ptr<Node>>>& solutions);

    static void coverDlxColumn(std::shared_ptr<Node>& column);
    
    static void uncoverDlxColumn(std::shared_ptr<Node>& column);

    static std::shared_ptr<Node> chooseSmallestColumn(const std::shared_ptr<Node>& root);
    
    static std::shared_ptr<Node> chooseRandomColumn(const std::shared_ptr<Node>& root);

    
    static void printDancingLinksMatrix(const std::shared_ptr<Node>& root,
                                        const std::vector<std::unique_ptr<AbstractConstraint>>& constraints,
                                        const std::vector<std::vector<Sudo>>& board,
                                        const std::vector<std::vector<bool>>& givenMask);
};
