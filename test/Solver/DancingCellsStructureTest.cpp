#include "DancingCellsStructure.hpp"

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

  SUBCASE("Construction") {

    SUBCASE("Primary and secondary items with colors") {
      const auto structure =
          DancingCellsStructure(3,
                                2,
                                {
                                    {{0, 1, {3, 3}, {4, 1}}}, // Option 0: 'p q x:C y:A'
                                    {{0, 2, {3, 1}, {4, 3}}}, // Option 1: 'p r x:A y:C' // Part of solution
                                    {{0, {3, 2}}}, // Option 2: 'p x:B'
                                    {{1, {3, 1}}}, // Option 3: 'q x:A' // Part of solution
                                    {{2, {4, 3}}}, // Option 4: 'r y:C'
                                });
      const auto expectedItem = std::vector<int32_t>{2, 7, 11, 15, 21};
      const auto expectedSet =
          std::vector<int32_t>{0, 3, 1, 6, 11, 1, 2, 2, 14, 2, 2, 7, 17, 3, 4, 3, 8, 12, 15, 4, 3, 4, 9, 18};
      const auto expectedNode = std::vector<DancingCellsNode>{
          {0, 4, 0},   {2, 2, 0},   {7, 7, 0},   {15, 15, 3}, {21, 21, 1}, {-4, 4, 0},  {2, 3, 0},
          {11, 11, 0}, {15, 16, 1}, {21, 22, 3}, {-4, 2, 0},  {2, 4, 0},   {15, 17, 2}, {-2, 2, 0},
          {7, 8, 0},   {15, 18, 1}, {-2, 2, 0},  {11, 12, 0}, {21, 23, 3}, {-2, 0, 0}};

      const auto expectedNodeOptionIndices =
          std::vector<int32_t>{0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 2, 2, 0, 3, 3, 0, 4, 4, 0};

      CHECK_EQ(expectedItem, structure.ITEM);
      CHECK_EQ(expectedSet, structure.SET);
      CHECK_EQ(expectedNode, structure.NODE);
      CHECK_EQ(expectedNodeOptionIndices, structure.nodeOptionIndices);
      CHECK_EQ(3, structure.primaryItemsCount);
      CHECK_EQ(2, structure.secondaryItemsCount);
      CHECK_EQ(5, structure.itemsCount);
      CHECK_EQ(5, structure.optionsCount);
    }
  }
}
