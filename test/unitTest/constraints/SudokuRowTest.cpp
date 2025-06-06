
#include "constraints/SudokuRow.hpp"

#include <doctest.h>

TEST_SUITE("Constraints: Output") {
  TEST_CASE("Sudoku-Row") {
    auto constraint = SudokuRow();
    constraint.initialize();
    const std::vector<std::vector<int32_t>> expectedPrimaryItems = {
        {0},  {1},  {2},  {3},  {4},  {5},  {6},  {7},  {8},  {0},  {1},  {2},  {3},  {4},  {5},  {6},  {7},  {8},
        {0},  {1},  {2},  {3},  {4},  {5},  {6},  {7},  {8},  {0},  {1},  {2},  {3},  {4},  {5},  {6},  {7},  {8},
        {0},  {1},  {2},  {3},  {4},  {5},  {6},  {7},  {8},  {0},  {1},  {2},  {3},  {4},  {5},  {6},  {7},  {8},
        {0},  {1},  {2},  {3},  {4},  {5},  {6},  {7},  {8},  {0},  {1},  {2},  {3},  {4},  {5},  {6},  {7},  {8},
        {0},  {1},  {2},  {3},  {4},  {5},  {6},  {7},  {8},  {9},  {10}, {11}, {12}, {13}, {14}, {15}, {16}, {17},
        {9},  {10}, {11}, {12}, {13}, {14}, {15}, {16}, {17}, {9},  {10}, {11}, {12}, {13}, {14}, {15}, {16}, {17},
        {9},  {10}, {11}, {12}, {13}, {14}, {15}, {16}, {17}, {9},  {10}, {11}, {12}, {13}, {14}, {15}, {16}, {17},
        {9},  {10}, {11}, {12}, {13}, {14}, {15}, {16}, {17}, {9},  {10}, {11}, {12}, {13}, {14}, {15}, {16}, {17},
        {9},  {10}, {11}, {12}, {13}, {14}, {15}, {16}, {17}, {9},  {10}, {11}, {12}, {13}, {14}, {15}, {16}, {17},
        {18}, {19}, {20}, {21}, {22}, {23}, {24}, {25}, {26}, {18}, {19}, {20}, {21}, {22}, {23}, {24}, {25}, {26},
        {18}, {19}, {20}, {21}, {22}, {23}, {24}, {25}, {26}, {18}, {19}, {20}, {21}, {22}, {23}, {24}, {25}, {26},
        {18}, {19}, {20}, {21}, {22}, {23}, {24}, {25}, {26}, {18}, {19}, {20}, {21}, {22}, {23}, {24}, {25}, {26},
        {18}, {19}, {20}, {21}, {22}, {23}, {24}, {25}, {26}, {18}, {19}, {20}, {21}, {22}, {23}, {24}, {25}, {26},
        {18}, {19}, {20}, {21}, {22}, {23}, {24}, {25}, {26}, {27}, {28}, {29}, {30}, {31}, {32}, {33}, {34}, {35},
        {27}, {28}, {29}, {30}, {31}, {32}, {33}, {34}, {35}, {27}, {28}, {29}, {30}, {31}, {32}, {33}, {34}, {35},
        {27}, {28}, {29}, {30}, {31}, {32}, {33}, {34}, {35}, {27}, {28}, {29}, {30}, {31}, {32}, {33}, {34}, {35},
        {27}, {28}, {29}, {30}, {31}, {32}, {33}, {34}, {35}, {27}, {28}, {29}, {30}, {31}, {32}, {33}, {34}, {35},
        {27}, {28}, {29}, {30}, {31}, {32}, {33}, {34}, {35}, {27}, {28}, {29}, {30}, {31}, {32}, {33}, {34}, {35},
        {36}, {37}, {38}, {39}, {40}, {41}, {42}, {43}, {44}, {36}, {37}, {38}, {39}, {40}, {41}, {42}, {43}, {44},
        {36}, {37}, {38}, {39}, {40}, {41}, {42}, {43}, {44}, {36}, {37}, {38}, {39}, {40}, {41}, {42}, {43}, {44},
        {36}, {37}, {38}, {39}, {40}, {41}, {42}, {43}, {44}, {36}, {37}, {38}, {39}, {40}, {41}, {42}, {43}, {44},
        {36}, {37}, {38}, {39}, {40}, {41}, {42}, {43}, {44}, {36}, {37}, {38}, {39}, {40}, {41}, {42}, {43}, {44},
        {36}, {37}, {38}, {39}, {40}, {41}, {42}, {43}, {44}, {45}, {46}, {47}, {48}, {49}, {50}, {51}, {52}, {53},
        {45}, {46}, {47}, {48}, {49}, {50}, {51}, {52}, {53}, {45}, {46}, {47}, {48}, {49}, {50}, {51}, {52}, {53},
        {45}, {46}, {47}, {48}, {49}, {50}, {51}, {52}, {53}, {45}, {46}, {47}, {48}, {49}, {50}, {51}, {52}, {53},
        {45}, {46}, {47}, {48}, {49}, {50}, {51}, {52}, {53}, {45}, {46}, {47}, {48}, {49}, {50}, {51}, {52}, {53},
        {45}, {46}, {47}, {48}, {49}, {50}, {51}, {52}, {53}, {45}, {46}, {47}, {48}, {49}, {50}, {51}, {52}, {53},
        {54}, {55}, {56}, {57}, {58}, {59}, {60}, {61}, {62}, {54}, {55}, {56}, {57}, {58}, {59}, {60}, {61}, {62},
        {54}, {55}, {56}, {57}, {58}, {59}, {60}, {61}, {62}, {54}, {55}, {56}, {57}, {58}, {59}, {60}, {61}, {62},
        {54}, {55}, {56}, {57}, {58}, {59}, {60}, {61}, {62}, {54}, {55}, {56}, {57}, {58}, {59}, {60}, {61}, {62},
        {54}, {55}, {56}, {57}, {58}, {59}, {60}, {61}, {62}, {54}, {55}, {56}, {57}, {58}, {59}, {60}, {61}, {62},
        {54}, {55}, {56}, {57}, {58}, {59}, {60}, {61}, {62}, {63}, {64}, {65}, {66}, {67}, {68}, {69}, {70}, {71},
        {63}, {64}, {65}, {66}, {67}, {68}, {69}, {70}, {71}, {63}, {64}, {65}, {66}, {67}, {68}, {69}, {70}, {71},
        {63}, {64}, {65}, {66}, {67}, {68}, {69}, {70}, {71}, {63}, {64}, {65}, {66}, {67}, {68}, {69}, {70}, {71},
        {63}, {64}, {65}, {66}, {67}, {68}, {69}, {70}, {71}, {63}, {64}, {65}, {66}, {67}, {68}, {69}, {70}, {71},
        {63}, {64}, {65}, {66}, {67}, {68}, {69}, {70}, {71}, {63}, {64}, {65}, {66}, {67}, {68}, {69}, {70}, {71},
        {72}, {73}, {74}, {75}, {76}, {77}, {78}, {79}, {80}, {72}, {73}, {74}, {75}, {76}, {77}, {78}, {79}, {80},
        {72}, {73}, {74}, {75}, {76}, {77}, {78}, {79}, {80}, {72}, {73}, {74}, {75}, {76}, {77}, {78}, {79}, {80},
        {72}, {73}, {74}, {75}, {76}, {77}, {78}, {79}, {80}, {72}, {73}, {74}, {75}, {76}, {77}, {78}, {79}, {80},
        {72}, {73}, {74}, {75}, {76}, {77}, {78}, {79}, {80}, {72}, {73}, {74}, {75}, {76}, {77}, {78}, {79}, {80},
        {72}, {73}, {74}, {75}, {76}, {77}, {78}, {79}, {80},
    };
    const std::vector<std::vector<int32_t>> expectedSecondaryItems = {};
    CHECK_EQ(expectedPrimaryItems, constraint.getPrimaryItems());
    CHECK_EQ(expectedSecondaryItems, constraint.getSecondaryItems());
  }
}
