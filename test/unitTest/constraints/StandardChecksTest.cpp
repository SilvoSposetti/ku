
#include "constraints/AntiKing.hpp"
#include "constraints/AntiKingTorus.hpp"
#include "constraints/AntiKnight.hpp"
#include "constraints/AntiKnightTorus.hpp"
#include "constraints/Asterisk.hpp"
#include "constraints/DisjointBoxes.hpp"
#include "constraints/HyperSudoku.hpp"
#include "constraints/NegativeDiagonal.hpp"
#include "constraints/NegativeDiagonalEven.hpp"
#include "constraints/NegativeDiagonalOdd.hpp"
#include "constraints/PositiveDiagonal.hpp"
#include "constraints/PositiveDiagonalEven.hpp"
#include "constraints/PositiveDiagonalOdd.hpp"
#include "constraints/SudokuBox.hpp"
#include "constraints/SudokuCell.hpp"
#include "constraints/SudokuColumn.hpp"
#include "constraints/SudokuRow.hpp"

#include <algorithm>
#include <doctest.h>

TEST_SUITE("Constraints: Standard Checks") {
  TEST_CASE_TEMPLATE_DEFINE("Standard Checks ", T, checks) {
    const int32_t optionsAmount = Sudo::INDICES.size() * Sudo::INDICES.size() * Sudo::SUDO_DIGITS.size();
    auto constraint = T();
    constraint.initialize();
    const int32_t primaryItemsAmount = constraint.getPrimaryItemsAmount();
    const auto primaryItems = constraint.getPrimaryItems();
    const int32_t secondaryItemsAmount = constraint.getSecondaryItemsAmount();
    const auto secondaryItems = constraint.getSecondaryItems();

    SUBCASE("Info is not empty") {
      CHECK(!constraint.getName().empty());
      CHECK(!constraint.getDescription().empty());
    }

    SUBCASE("Amount of primary items") {
      // If primary items are defined, then there should be at least one for every option
      if (primaryItemsAmount == 0) {
        CHECK_EQ(0, primaryItems.size());
      } else {
        CHECK_EQ(optionsAmount, primaryItems.size());
      }
    }

    SUBCASE("Amount of secondary items") {
      // If secondary items are defined, then there should be at least one for every option
      if (secondaryItemsAmount == 0) {
        CHECK_EQ(0, secondaryItems.size());
      } else {
        CHECK_EQ(optionsAmount, secondaryItems.size());
      }
    }

    SUBCASE("Primary item IDs") {
      // Item IDs in the primary items list cannot be larger than the amount defined
      CHECK(std::all_of(primaryItems.begin(), primaryItems.end(), [&](const std::vector<int32_t> optionItems) {
        return std::all_of(optionItems.begin(), optionItems.end(), [&](int32_t itemId) {
          return 0 <= itemId && itemId < primaryItemsAmount;
        });
      }));
    }

    SUBCASE("Secondary item IDs") {
      // Item IDs in the secondary items list cannot be larger than the amount defined
      CHECK(std::all_of(secondaryItems.begin(), secondaryItems.end(), [&](const std::vector<int32_t> optionItems) {
        return std::all_of(optionItems.begin(), optionItems.end(), [&](int32_t itemId) {
          return 0 <= itemId && itemId < secondaryItemsAmount;
        });
      }));
    }
  }

  // Invocations
  TEST_CASE_TEMPLATE_INVOKE(checks, AntiKing);
  TEST_CASE_TEMPLATE_INVOKE(checks, AntiKingTorus);
  TEST_CASE_TEMPLATE_INVOKE(checks, AntiKnight);
  TEST_CASE_TEMPLATE_INVOKE(checks, AntiKnightTorus);
  TEST_CASE_TEMPLATE_INVOKE(checks, Asterisk);
  TEST_CASE_TEMPLATE_INVOKE(checks, DisjointBoxes);
  TEST_CASE_TEMPLATE_INVOKE(checks, HyperSudoku);
  TEST_CASE_TEMPLATE_INVOKE(checks, NegativeDiagonal);
  TEST_CASE_TEMPLATE_INVOKE(checks, NegativeDiagonalEven);
  TEST_CASE_TEMPLATE_INVOKE(checks, NegativeDiagonalOdd);
  TEST_CASE_TEMPLATE_INVOKE(checks, PositiveDiagonal);
  TEST_CASE_TEMPLATE_INVOKE(checks, PositiveDiagonalEven);
  TEST_CASE_TEMPLATE_INVOKE(checks, PositiveDiagonalOdd);
  TEST_CASE_TEMPLATE_INVOKE(checks, SudokuBox);
  TEST_CASE_TEMPLATE_INVOKE(checks, SudokuCell);
  TEST_CASE_TEMPLATE_INVOKE(checks, SudokuColumn);
  TEST_CASE_TEMPLATE_INVOKE(checks, SudokuRow);
}
