#include "GridUtilities.hpp"

#include <doctest.h>
#include <print>

TEST_CASE("Grid Utilities") {

  SUBCASE("Grid As Text") {

    SUBCASE("Single-Digit Numbers") {
      constexpr auto digits = 3;
      SUBCASE("1x1 Invalid") {
        constexpr auto space = PuzzleSpace{1, 1, digits};
        constexpr auto grid = Grid<space>{{{0}}};
        const auto expected = std::vector<std::string>{
            "┏━━━┓",
            "┃ ◌ ┃",
            "┗━━━┛",
        };
        const auto computed = GridUtilities::gridAsText<space>(grid);
        CHECK_EQ(computed, expected);
      }

      SUBCASE("1x1 Valid") {
        constexpr auto space = PuzzleSpace{1, 1, digits};
        constexpr auto grid = Grid<space>{{{2}}};
        const auto expected = std::vector<std::string>{
            "┏━━━┓",
            "┃ 2 ┃",
            "┗━━━┛",
        };
        const auto computed = GridUtilities::gridAsText<space>(grid);
        CHECK_EQ(computed, expected);
      }

      SUBCASE("3x4 All Invalid") {
        constexpr auto space = PuzzleSpace{3, 4, digits};
        constexpr auto grid = Grid<space>{{
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
        }};
        const auto expected = std::vector<std::string>{
            "┏━━━━━━━━━┓",
            "┃ ◌ ◌ ◌ ◌ ┃",
            "┃ ◌ ◌ ◌ ◌ ┃",
            "┃ ◌ ◌ ◌ ◌ ┃",
            "┗━━━━━━━━━┛",
        };
        const auto computed = GridUtilities::gridAsText<space>(grid);
        CHECK_EQ(computed, expected);
      }

      SUBCASE("3x4 Some Valid") {
        constexpr auto space = PuzzleSpace{3, 4, digits};
        constexpr auto grid = Grid<space>{{
            {1, 0, 2, 0},
            {0, 2, 0, 0},
            {3, 1, 2, 1},
        }};
        const auto expected = std::vector<std::string>{
            "┏━━━━━━━━━┓",
            "┃ 1 ◌ 2 ◌ ┃",
            "┃ ◌ 2 ◌ ◌ ┃",
            "┃ 3 1 2 1 ┃",
            "┗━━━━━━━━━┛",
        };
        const auto computed = GridUtilities::gridAsText<space>(grid);
        CHECK_EQ(computed, expected);
      }
    }

    SUBCASE("Multiple-Digit Numbers") {
      constexpr auto digits = 13;
      SUBCASE("1x1 Invalid") {
        constexpr auto space = PuzzleSpace{1, 1, digits};
        constexpr auto grid = Grid<space>{{{0}}};
        const auto expected = std::vector<std::string>{
            "┏━━━━┓",
            "┃  ◌ ┃",
            "┗━━━━┛",
        };
        const auto computed = GridUtilities::gridAsText<space>(grid);
        CHECK_EQ(computed, expected);
      }

      SUBCASE("1x1 Valid Single") {
        constexpr auto space = PuzzleSpace{1, 1, digits};
        constexpr auto grid = Grid<space>{{{2}}};
        const auto expected = std::vector<std::string>{
            "┏━━━━┓",
            "┃  2 ┃",
            "┗━━━━┛",
        };
        const auto computed = GridUtilities::gridAsText<space>(grid);
        CHECK_EQ(computed, expected);
      }

      SUBCASE("1x1 Valid Multiple") {
        constexpr auto space = PuzzleSpace{1, 1, digits};
        constexpr auto grid = Grid<space>{{{12}}};
        const auto expected = std::vector<std::string>{
            "┏━━━━┓",
            "┃ 12 ┃",
            "┗━━━━┛",
        };
        const auto computed = GridUtilities::gridAsText<space>(grid);
        CHECK_EQ(computed, expected);
      }

      SUBCASE("3x4 All Invalid") {
        constexpr auto space = PuzzleSpace{3, 4, digits};
        constexpr auto grid = Grid<space>{{
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
        }};
        const auto expected = std::vector<std::string>{
            "┏━━━━━━━━━━━━━┓",
            "┃  ◌  ◌  ◌  ◌ ┃",
            "┃  ◌  ◌  ◌  ◌ ┃",
            "┃  ◌  ◌  ◌  ◌ ┃",
            "┗━━━━━━━━━━━━━┛",
        };
        const auto computed = GridUtilities::gridAsText<space>(grid);
        CHECK_EQ(computed, expected);
      }

      SUBCASE("3x4 Some Valid") {
        constexpr auto space = PuzzleSpace{3, 4, digits};
        constexpr auto grid = Grid<space>{{
            {10, 0, 12, 0},
            {0, 7, 0, 0},
            {3, 11, 2, 1},
        }};
        const auto expected = std::vector<std::string>{
            "┏━━━━━━━━━━━━━┓",
            "┃ 10  ◌ 12  ◌ ┃",
            "┃  ◌  7  ◌  ◌ ┃",
            "┃  3 11  2  1 ┃",
            "┗━━━━━━━━━━━━━┛",
        };
        const auto computed = GridUtilities::gridAsText<space>(grid);
        CHECK_EQ(computed, expected);
      }
    }
  }
}
