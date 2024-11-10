#include "solver/AlgorithmC.h"

#include "doctest.h"
#include "solver/DancingCellsStructure.h"

#include <algorithm>

struct ProblemData {
  ProblemData(const DancingCellsStructure& structure, const std::vector<std::unordered_set<int32_t>>& expectedSolutions)
      : structure(structure)
      , expectedSolutions(expectedSolutions) {};

  const DancingCellsStructure structure;
  const std::vector<std::unordered_set<int32_t>> expectedSolutions;
};

void check(const std::vector<ProblemData>& problemsData, const std::vector<std::optional<int32_t>>& seeds) {
  for (const auto& seed : seeds) {
    for (const auto& [structure, expectedSolutions] : problemsData) {
      {
        // Uniqueness
        auto structureCopy = structure;
        if (expectedSolutions.size() == 1) {
          CHECK(AlgorithmC::hasUniqueSolution(structureCopy, seed));
        } else {
          auto structureCopy = structure;
          CHECK(!AlgorithmC::hasUniqueSolution(structureCopy, seed));
        }
      }

      {
        // Find all solutions
        auto structureCopy = structure;
        const auto allSolutionsFound = AlgorithmC::findAllSolutions(structureCopy, seed);
        // Check that every solution found was expected
        CHECK_EQ(allSolutionsFound.size(), expectedSolutions.size());
        for (const auto& solutionFound : allSolutionsFound) {
          CHECK(std::count(expectedSolutions.begin(), expectedSolutions.end(), solutionFound) == 1);
        }
      }

      {
        // Find one solution
        auto structureCopy = structure;
        // Check that the one solution found appears in the list of the ones provided
        const auto oneSolutionOptional = AlgorithmC::findOneSolution(structureCopy, seed);
        if (expectedSolutions.empty()) {
          CHECK(!oneSolutionOptional.has_value());
        } else {
          CHECK(oneSolutionOptional.has_value());
          CHECK(std::count(expectedSolutions.begin(), expectedSolutions.end(), oneSolutionOptional.value()) == 1);
        }
      }
    }
  }
}

TEST_CASE("Algorithm C") {
  const std::vector<std::optional<int32_t>> seeds = {std::nullopt, 0, 1, -566, 9845};

  SUBCASE("Preconditions") {
    // No primary items
    CHECK_THROWS(DancingCellsStructure(0, 2, {}));

    // TODO: Should it really throw if there are no options?
    CHECK_THROWS(DancingCellsStructure(2, 2, {}));

    // No options that cover a primary item
    CHECK_THROWS(DancingCellsStructure(2,
                                       1,
                                       {
                                           {0},
                                           {0, 2},
                                       }));

    // Primary item id outside of the valid range
    CHECK_THROWS(DancingCellsStructure(2, 2, {{2, 0}}));

    // Secondary item id outside of the valid range
    CHECK_THROWS(DancingCellsStructure(2, 2, {{0, 0}}));
  }

  SUBCASE("Single solution, primary and secondary items with colors") {
    const int32_t primaryItemsCount = 3; // p q r
    const int32_t secondaryItemsCount = 2; // x y
    const std::vector<std::vector<XccElement>> options = {
        {{0, 1, {3, 3}, {4, 1}}}, // Option 0: 'p q x:C y:A'
        {{0, 2, {3, 1}, {4, 3}}}, // Option 1: 'p r x:A y:C' // Part of solution
        {{0, {3, 2}}}, // Option 2: 'p x:B'
        {{1, {3, 1}}}, // Option 3: 'q x:A' // Part of solution
        {{2, {4, 3}}}, // Option 4: 'r y:C'
    };
    const std::unordered_set<int32_t> solution = {1, 3};

    for (const auto& seed : seeds) {
      auto structure = DancingCellsStructure(primaryItemsCount, secondaryItemsCount, options);
      // REQUIRE(structure.isPotentiallySolvableByAlgorithmX());
      CHECK(AlgorithmC::hasUniqueSolution(structure, seed));
      const auto allSolutions = AlgorithmC::findAllSolutions(structure, seed);
      CHECK(allSolutions.size() == 1);
      CHECK_EQ(*allSolutions.begin(), solution);
      const auto oneSolutionOptional = AlgorithmC::findOneSolution(structure, seed);
      CHECK(oneSolutionOptional.has_value());
      CHECK_EQ(oneSolutionOptional, solution);
    }
  }

  SUBCASE("No solutions, primary items only") {
    const std::vector<ProblemData> problemsData = {

        // Problem 1
        {
            {3, // Primary items
             0, // Secondary items
             {
                 /*{0, 1, 1}*/ {1, 2},
                 /*{1, 1, 0}*/ {0, 1},
             }},
            {} // No solutions
        },

        // Promblem 2
        {
            {3, // Primary items
             0, // Secondary items
             {
                 /*{1, 0, 1}*/ {0, 2},
                 /*{0, 0, 0}*/ {}, // Empty option
                 /*{0, 1, 1}*/ {1, 2},
                 /*{1, 1, 0}*/ {0, 1},
                 /*{0, 0, 0}*/ {}, // Empty option
             }},
            {} // No solutions
        },

        // Problem 3
        {
            {7, // Primary items
             0, // Secondary items
             {
                 /*{0, 0, 1, 0, 1, 0, 0}*/ {2, 4},
                 /*{1, 0, 0, 1, 0, 0, 1}*/ {0, 3, 6},
                 /*{0, 1, 1, 0, 0, 1, 0}*/ {1, 2, 5},
                 /*{1, 0, 0, 1, 0, 0, 0}*/ {0, 3},
                 /*{0, 1, 0, 0, 0, 0, 1}*/ {2, 6},
                 /*{0, 0, 0, 1, 1, 0, 1}*/ {3, 4, 6},
             }},
            {} // No solutions
        },

        // Problem 4
        {
            {7, // Primary items
             0, // Secondary items
             {
                 /*{1, 1, 0, 1, 1, 1, 1}*/ {0, 1, 3, 4, 5, 6},
                 /*{0, 1, 1, 0, 0, 0, 0}*/ {1, 2},
                 /*{0, 0, 1, 1, 0, 0, 0}*/ {2, 3},
                 /*{0, 0, 0, 0, 1, 1, 0}*/ {4, 5},
                 /*{0, 0, 0, 1, 0, 0, 0}*/ {3},
                 /*{0, 0, 0, 0, 0, 0, 0}*/ {}, // Empty option
                 /*{1, 1, 1, 0, 0, 0, 0}*/ {0, 1, 2},
             }},
            {} // No solutions
        },

    };

    check(problemsData, seeds);
  }

  SUBCASE("Single solution, primary items only") {
    const std::vector<ProblemData> problemsData = {

        // Problem 1
        {{1, // Primary items
          0, // Secondary items
          {
              /*{1}*/ {0}, // Part of solution
          }},
         // Solution
         {{0}}},

        // Problem 2
        {{2, // Primary items
          0, // Secondary items
          {
              /*{1, 1}*/ {0, 1} // Part of solution
          }},
         // Solution
         {{0}}},

        // Problem 3
        {{6, // Primary items
          0, // Secondary items
          {
              /*{0, 0, 0, 0, 0, 0}*/ {}, // Empty option
              /*{1, 1, 1, 1, 1, 1}*/ {0, 1, 2, 3, 4, 5}, // Part of solution
          }},
         // Solution
         {{1}}},

        // Problem 4
        {{7, // Primary items
          0, // Secondary items
          {
              /*{0, 0, 1, 0, 1, 0, 0}*/ {2, 4}, // Part of solution
              /*{1, 0, 0, 1, 0, 0, 1}*/ {0, 3, 6},
              /*{0, 1, 1, 0, 0, 1, 0}*/ {1, 2, 5},
              /*{1, 0, 0, 1, 0, 1, 0}*/ {0, 3, 5}, // Part of solution
              /*{0, 1, 0, 0, 0, 0, 1}*/ {1, 6}, // Part of solution
              /*{0, 0, 0, 1, 1, 0, 1}*/ {3, 4, 6},
          }},
         // Solution
         {{0, 3, 4}}},

        // Problem 5
        {{7, // Primary items
          0, // Secondary items
          {
              /*{1, 0, 0, 1, 0, 0, 1}*/ {0, 3, 6},
              /*{1, 0, 0, 1, 0, 0, 0}*/ {0, 3}, // Part of solution
              /*{0, 0, 0, 1, 1, 0, 1}*/ {3, 4, 6},
              /*{0, 0, 1, 0, 1, 1, 0}*/ {2, 4, 5}, // Part of solution
              /*{0, 1, 1, 0, 0, 1, 1}*/ {1, 2, 5, 6},
              /*{0, 1, 0, 0, 0, 0, 1}*/ {1, 6}, // Part of solution
              /*{0, 0, 0, 0, 0, 0, 0}*/ {}, // Empty option
          }},
         // Solution
         {{1, 3, 5}}},

        // Problem 6
        {{6, // Primary items
          0, // Secondary items
          {
              /*{1, 1, 0, 0, 0, 0}*/ {0, 1}, // Part of solution
              /*{0, 0, 0, 0, 1, 1}*/ {4, 5}, // Part of solution
              /*{0, 0, 0, 1, 1, 0}*/ {3, 4},
              /*{1, 0, 1, 0, 0, 0}*/ {0, 2},
              /*{0, 0, 0, 0, 0, 0}*/ {}, // Empty option
              /*{0, 0, 1, 0, 0, 0}*/ {2}, // Part of solution
              /*{0, 0, 0, 1, 0, 0}*/ {3}, // Part of solution
              /*{1, 0, 1, 0, 0, 1}*/ {0, 2, 5},
          }},
         // Solution
         {{0, 1, 5, 6}}},
    };

    check(problemsData, seeds);
  }

  SUBCASE("Single solution, primary and secondary items") {
    const std::vector<ProblemData> problemsData = {
        // Problem 1
        {{4, // Primary items
          3, // Secondary items
          {
              /*{1, 0, 0, 0 | 0, 0, 0}*/ {0}, // Part of solution
              /*{0, 1, 0, 0 | 1, 0, 0}*/ {1, 4}, // Part of solution
              /*{0, 1, 0, 0 | 0, 1, 0}*/ {1, 5},
              /*{0, 0, 1, 0 | 1, 1, 0}*/ {2, 4, 5},
              /*{0, 0, 1, 0 | 0, 1, 1}*/ {2, 5, 6},
              /*{0, 0, 1, 0 | 0, 1, 0}*/ {2, 5}, // Part of solution
              /*{0, 0, 0, 1 | 0, 0, 1}*/ {3, 6}, // Part of solution
          }},
         // Solution
         {{0, 1, 5, 6}}},

        // Problem 2
        {{3, // Primary items
          1, // Secondary items
          {
              /*{0, 1, 0 | 1}*/ {1, 3},
              /*{0, 1, 0 | 0}*/ {1}, // Part of solution
              /*{1, 0, 0 | 0}*/ {0}, // Part of solution
              /*{0, 0, 0 | 0}*/ {}, // Empty option
              /*{0, 0, 1 | 1}*/ {2, 3}, // Part of solution
              /*{1, 0, 0 | 1}*/ {0, 3},
          }},
         // Solution
         {{1, 2, 4}}},

        // // Problem 3 (secondary items given, but no option with them)
        // {4, // Primary items
        //  2, // Secondary items
        //  {
        //      /*{1, 0, 0, 0 | 0, 0}*/ {{0}}, // Part of solution
        //      /*{0, 0, 1, 0 | 0, 0}*/ {{2}},
        //      /*{0, 0, 0, 1 | 0, 0}*/ {{3}}, // Part of solution
        //      /*{0, 0, 1, 1 | 0, 0}*/ {{2, 3}},
        //      /*{0, 1, 1, 0 | 0, 0}*/ {{1, 2}}, // Part of solution
        //  },
        //  // Solution
        //  {0, 2, 4}},

        // Problem 4
        {{2, // Primary items
          3, // Secondary items
          {
              /*{0, 0 | 0, 1, 0}*/ {3}, // No primary items, this will never be selected
              /*{1, 0 | 1, 0, 0}*/ {0, 2}, // Part of solution
              /*{0, 0 | 0, 0, 0}*/ {}, // Empty option
              /*{0, 1 | 0, 0, 1}*/ {1, 4}, // Part of solution
          }},
         // Solution
         {{1, 3}}},

    };

    check(problemsData, seeds);
  }

  SUBCASE("Multiple solutions, primary items only") {
    const std::vector<ProblemData> problemsData = {
        // Problem 1
        {{7, // Primary items
          0, // Secondary items
          {
              /*{1, 0, 0, 1, 0, 0, 1},*/ {0, 3, 6},
              /*{1, 0, 0, 1, 0, 1, 0},*/ {0, 3, 5}, // Part of solution A
              /*{0, 1, 0, 0, 1, 1, 0},*/ {1, 4, 5}, // Part of solution B
              /*{0, 1, 1, 0, 0, 1, 0},*/ {1, 2, 5},
              /*{0, 0, 1, 1, 0, 0, 0},*/ {2, 3}, // Part of solution B
              /*{0, 0, 1, 0, 1, 0, 0},*/ {2, 4}, // Part of solution A
              /*{0, 1, 0, 0, 0, 0, 1},*/ {1, 6}, // Part of solution A
              /*{0, 0, 0, 1, 1, 0, 1},*/ {3, 4, 6},
              /*{0, 0, 0, 0, 0, 0, 0},*/ {}, // Empty option
              /*{1, 0, 0, 0, 0, 0, 1},*/ {0, 6}, // Part of solution B
              /*{0, 0, 1, 0, 0, 1, 0},*/ {2, 5},
          }},
         // Solutions
         {
             {1, 5, 6}, // Solution A
             {2, 4, 9}, // Solution B
         }},

        // Problem 2
        {{4, // Primary items
          0, // Secondary items
          {
              /*{1, 0, 0, 0}*/ {0}, // Part of solution A | B
              /*{0, 1, 0, 0}*/ {1}, // Part of solution A
              /*{0, 0, 1, 0}*/ {2}, // Part of solution A | C
              /*{0, 0, 0, 1}*/ {3}, // Part of solution A | B | C
              /*{0, 1, 1, 0}*/ {1, 2}, // Part of solution B
              /*{1, 1, 0, 0}*/ {0, 1}, // Part of solution C
          }},
         // Solutions
         {
             {0, 1, 2, 3}, // Solution A
             {0, 3, 4}, // Solution B
             {2, 3, 5}, // Solution C
         }},

        // Problem 3
        {{3, // Primary items
          0, // Secondary items
          {
              /*{0, 0, 0}*/ {}, // Empty option
              /*{1, 0, 0}*/ {0}, // Solution A | B
              /*{0, 1, 0}*/ {1}, // Solution A | C
              /*{0, 0, 1}*/ {2}, // Solution A | D
              /*{1, 1, 0}*/ {0, 1}, // Solution D
              /*{1, 0, 1}*/ {0, 2}, // Solution C
              /*{0, 1, 1}*/ {1, 2}, // Solution B
              /*{1, 1, 1}*/ {0, 1, 2}, // Solution E
          }},
         // Solutions
         {
             {1, 2, 3}, // Solution A
             {1, 6}, // Solution B
             {4, 3}, // Solution C
             {5, 2}, // Solution D
             {7}, // Solution E
         }},

        // Problem 4
        {{2, // Primary items
          0, // Secondary items
          {
              /*{1, 0}*/ {0}, // Solution A | B | C
              /*{1, 0}*/ {0}, // Solution D | E | F
              /*{1, 0}*/ {0}, // Solution G | H | I
              /*{0, 1}*/ {1}, // Solution A | D | G
              /*{0, 1}*/ {1}, // Solution B | E | H
              /*{0, 1}*/ {1}, // Solution C | F | I
          }},
         // Solutions
         {
             {0, 3}, // Solution A
             {0, 4}, // Solution B
             {0, 5}, // Solution C
             {1, 3}, // Solution D
             {1, 4}, // Solution E
             {1, 5}, // Solution F
             {2, 3}, // Solution G
             {2, 4}, // Solution H
             {2, 5}, // Solution I
         }},

        // Problem 5
        {{1, // Primary items
          0, // Secondary items
          {
              /*{1}*/ {0}, // Solution A
              /*{1}*/ {0}, // Solution B
              /*{1}*/ {0}, // Solution C
          }},
         // Solutions
         {
             {0}, // Solution A
             {1}, // Solution B
             {2}, // Solution C
         }},
    };

    check(problemsData, seeds);
  }

  SUBCASE("Multiple solutions, primary and secondary items") {
    const std::vector<ProblemData> problemsData = {

        // Problem 1
        {{4, // Primary items
          3, // Secondary items
          {
              /*{0, 0, 1, 0 | 1, 0, 0}*/ {2, 4}, // Part of solution A
              /*{1, 0, 0, 1 | 0, 0, 1}*/ {0, 3, 6}, // Part of solution B
              /*{0, 1, 1, 0 | 0, 1, 0}*/ {1, 2, 5}, // Part of solution B
              /*{1, 0, 0, 1 | 0, 1, 0}*/ {0, 3, 5}, // Part of solution A
              /*{0, 1, 0, 0 | 0, 0, 1}*/ {1, 6}, // Part of solution A
              /*{0, 0, 0, 1 | 1, 0, 1}*/ {3, 4, 6},
          }},
         // Solutions
         {
             {0, 3, 4}, // Solution A
             {1, 2}, // Solution B
         }},

        // Problem 2
        {{3, // Primary items
          1, // Secondary items
          {
              /*{0, 0, 1 | 0}*/ {2}, // Part of solution A
              /*{1, 0, 0 | 1}*/ {0, 3}, // Part of solution B
              /*{0, 1, 1 | 0}*/ {1, 2}, // Part of solution B
              /*{0, 0, 0 | 0}*/ {}, // Empty option
              /*{1, 1, 0 | 0}*/ {0, 1}, // Part of solution A
          }},
         // Solutions
         {
             {0, 4}, // Solution A
             {1, 2}, // Solution B
         }},

        // Problem 3
        {{3, // Primary items
          2, // Secondary items
          {
              /*{0, 1, 0 | 0, 0}*/ {1}, //  Part of solution A
              /*{1, 0, 1 | 0, 1}*/ {0, 2, 4}, //  Part of solution A | B | C
              /*{0, 0, 0 | 0, 0}*/ {}, // Empty option
              /*{0, 1, 0 | 1, 1}*/ {1, 3, 4},
              /*{0, 1, 0 | 1, 0}*/ {1, 3}, //  Part of solution B
              /*{0, 1, 0 | 0, 0}*/ {1}, //  Part of solution C
          }},
         // Solutions
         {
             {0, 1}, // Solution A
             {1, 4}, // Solution B
             {1, 5}, // Solution C
         }},
    };

    check(problemsData, seeds);
  }

  //   SUBCASE("Algorithm X Solvability") {
  //     // Create & fill. Note that there's no column with all cells unset
  //     const std::vector<std::vector<bool>> matrix = {
  //         {1, 0, 1, 0, 0, 0},
  //         {1, 1, 0, 1, 0, 0},
  //         {0, 1, 1, 0, 1, 0},
  //         {0, 0, 1, 1, 0, 1},
  //         {0, 0, 0, 0, 0, 0},
  //         {1, 1, 0, 0, 0, 1},
  //     };
  //     const int32_t columns = matrix.at(0).size();

  //     SUBCASE("Solvable") {
  //       // Nothing out of the ordinary
  //       DataStructure dataStructure(matrix);
  //       CHECK(dataStructure.isPotentiallySolvableByAlgorithmX());
  //     }

  //     SUBCASE("All columns secondary") {
  //       // Sparse matrix is not solvable by Algorithm X if all columns are secondary
  //       DataStructure allSecondaryColumnsDataStructure(matrix, columns);
  //       CHECK_FALSE(allSecondaryColumnsDataStructure.isPotentiallySolvableByAlgorithmX());
  //     }

  //     SUBCASE("Empty primary columns") {
  //       {
  //         // Matrix with an all-zeroes primary column in the middle
  //         const std::vector<std::vector<bool>> invalidMatrix = {
  //             {1, 0, 1, 0, 0, 0},
  //             {1, 1, 0, 0, 1, 0},
  //             {0, 1, 1, 0, 0, 0},
  //             {0, 0, 1, 0, 1, 1},
  //             {0, 0, 0, 0, 0, 0},
  //             {1, 1, 0, 0, 0, 1},
  //         };

  //         DataStructure unsolvableDataStructure(invalidMatrix);
  //         // Matrix is unsolvable, since a primary column is empty
  //         CHECK_FALSE(unsolvableDataStructure.isPotentiallySolvableByAlgorithmX());
  //       }

  //       {
  //         // Matrix with two all-zeroes columns at the end
  //         const std::vector<std::vector<bool>> invalidMatrix = {
  //             {1, 0, 1, 0, 0, 0, 0},
  //             {1, 1, 0, 1, 0, 0, 0},
  //             {0, 1, 1, 0, 0, 0, 0},
  //             {0, 0, 1, 1, 1, 0, 0},
  //             {0, 0, 0, 0, 0, 0, 0},
  //             {1, 1, 0, 0, 1, 0, 0},
  //         };

  //         DataStructure unsolvableDataStructure(invalidMatrix);
  //         // Now matrix is unsolvable, since a primary column is empty
  //         CHECK_FALSE(unsolvableDataStructure.isPotentiallySolvableByAlgorithmX());

  //         // But if the last empty columns are set to be secondary the matrix becomes solvable
  //         DataStructure solvableDataStructure(invalidMatrix, 2);
  //         CHECK(solvableDataStructure.isPotentiallySolvableByAlgorithmX());
  //       }
  //     }

  //     // Empty matrix is solvable
  //     const DataStructure empty({});
  //     CHECK(empty.isPotentiallySolvableByAlgorithmX());
  //   }
}
