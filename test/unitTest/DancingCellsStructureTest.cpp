#include "solver/DancingCellsStructure.h"

#include <doctest.h>

TEST_CASE("Dancing Cells Structure") {
  const std::vector<std::optional<int32_t>> seeds = {std::nullopt, 0, 1, -566, 9845};

  SUBCASE("Preconditions") {

    SUBCASE("No primary items") {
      CHECK_THROWS(DancingCellsStructure(0, 2, {{0, 1}}));
      CHECK_THROWS(DancingCellsStructure(0, // Primary items
                                         6, // Secondary items
                                         {
                                             /*{1, 0, 1, 0, 0, 0}*/ {0, 2},
                                             /*{1, 1, 0, 1, 0, 0}*/ {0, 1, 3},
                                             /*{0, 1, 1, 0, 1, 0}*/ {1, 2, 4},
                                             /*{0, 0, 1, 1, 0, 1}*/ {2, 3, 5},
                                             /*{0, 0, 0, 0, 0, 0}*/ {},
                                             /*{1, 1, 0, 0, 0, 1}*/ {0, 1, 5},
                                         }));
    }

    SUBCASE("No options") {
      CHECK_THROWS(DancingCellsStructure(1, 2, {}));
      CHECK_THROWS(DancingCellsStructure(2, 2, {}));
    }

    SUBCASE("Some item cannot be covered") {

      CHECK_THROWS(DancingCellsStructure(2,
                                         1,
                                         {
                                             {0},
                                             {0, 2},
                                         }));
      // One primary item in the middle cannot be covered
      CHECK_THROWS(DancingCellsStructure(6, // Primary items
                                         0, // Secondary items
                                         {
                                             /*{1, 0, 1, 0, 0, 0}*/ {0, 2},
                                             /*{1, 1, 0, 0, 1, 0}*/ {0, 1, 4},
                                             /*{0, 1, 1, 0, 0, 0}*/ {1, 2},
                                             /*{0, 0, 1, 0, 1, 1}*/ {2, 4, 5},
                                             /*{0, 0, 0, 0, 0, 0}*/ {},
                                             /*{1, 1, 0, 0, 0, 1}*/ {0, 1, 5},
                                         }));
      // Two primary items at the end cannot be covered
      CHECK_THROWS(DancingCellsStructure(7, // Primary items
                                         0, // Secondary items
                                         {
                                             /*{1, 0, 1, 0, 0, 0, 0}*/ {0, 2},
                                             /*{1, 1, 0, 1, 0, 0, 0}*/ {0, 1, 3},
                                             /*{0, 1, 1, 0, 0, 0, 0}*/ {1, 2},
                                             /*{0, 0, 1, 1, 1, 0, 0}*/ {2, 3, 4},
                                             /*{0, 0, 0, 0, 0, 0, 0}*/ {},
                                             /*{1, 1, 0, 0, 1, 0, 0}*/ {0, 1, 4},
                                         }));
      // Two secondary items at the end cannot be covered
      CHECK_THROWS(DancingCellsStructure(2, // Primary items
                                         5, // Secondary items
                                         {
                                             /*{1, 0 | 1, 0, 0, 0, 0}*/ {0, 2},
                                             /*{1, 1 | 0, 1, 0, 0, 0}*/ {0, 1, 3},
                                             /*{0, 1 | 1, 0, 0, 0, 0}*/ {1, 2},
                                             /*{0, 0 | 1, 1, 1, 0, 0}*/ {2, 3, 4},
                                             /*{0, 0 | 0, 0, 0, 0, 0}*/ {},
                                             /*{1, 1 | 0, 0, 1, 0, 0}*/ {0, 1, 4},
                                         }));
    }

    SUBCASE("Primary or secondary items outside of valid range") {
      // Primary item id outside of the valid range
      CHECK_THROWS(DancingCellsStructure(2, 2, {{2, 0}}));
      // Secondary item id outside of the valid range
      CHECK_THROWS(DancingCellsStructure(2, 2, {{0, 0}}));
    }

    SUBCASE("Valid") {
      // All items could be covered
      CHECK_NOTHROW(DancingCellsStructure(6, // Primary items
                                          0, // Secondary items
                                          {
                                              /*{1, 0, 1, 0, 0, 0},*/ {0, 2},
                                              /*{1, 1, 0, 1, 0, 0},*/ {0, 1, 3},
                                              /*{0, 1, 1, 0, 1, 0},*/ {1, 2, 4},
                                              /*{0, 0, 1, 1, 0, 1},*/ {2, 3, 4},
                                              /*{0, 0, 0, 0, 0, 0},*/ {},
                                              /*{1, 1, 0, 0, 0, 1},*/ {0, 1, 5},
                                          }));
    }
  }

}
