#include "Board.h"

#include <iostream>
#include <string>

Board::Board(const std::vector<std::vector<Sudo::Digit>>& solution, const std::vector<std::vector<bool>>& givenMask)
    : solution(solution)
    , givenMask(givenMask) {}

const std::vector<std::vector<Sudo::Digit>>& Board::getSolution() const {
  return solution;
}

const std::vector<std::vector<bool>>& Board::getGivenMask() const {
  return givenMask;
}

std::vector<std::vector<Sudo::Digit>> Board::getField() const {
  std::vector<std::vector<Sudo::Digit>> field = solution;
  for (const auto& i : Sudo::INDICES) {
    for (const auto& j : Sudo::INDICES) {
      if (!givenMask[i][j]) {
        field[i][j] = Sudo::Digit::NONE;
      }
    }
  }
  return field;
}

void Board::printGivenPattern() const {
  for (const auto& row : givenMask) {
    for (const auto& element : row) {
      const std::string str = element ? "■" : " ";
      std::cout << str << " ";
    }
    std::cout << std::endl;
  }
}

void Board::print() const {
  // Colors:
  const char normalColor[] = {0x1b, '[', '0', ';', '3', '9', 'm', 0};
  const char solvedColor[] = {0x1b, '[', '0', ';', '3', '7', 'm', 0};

  std::vector<std::vector<Sudo::Digit>> field = getField();
  for (const auto& i : Sudo::INDICES) {
    if (i == 0) {
      std::cout << normalColor << "┏━━━━━━━┯━━━━━━━┯━━━━━━━┓" << std::endl;
    }
    for (const auto& j : Sudo::INDICES) {
      if (j == 0) {
        std::cout << normalColor + std::string("┃ ");
      }

      std::string c;
      if (givenMask[i][j])
        c = normalColor + std::to_string(static_cast<int32_t>(solution[i][j]));
      else if (field[i][j] != Sudo::Digit::NONE)
        c = solvedColor + std::to_string(static_cast<int32_t>(field[i][j]));
      else
        c = solvedColor + std::string("◌");
      std::cout << c;

      if (j < 8) {
        if (j % 3 == 2)
          std::cout << normalColor << " │ ";
        else
          std::cout << normalColor << " ";
      }
      if (j == 8)
        std::cout << normalColor + std::string(" ┃");
    }
    std::cout << normalColor << std::endl;
    if (i % 3 == 2 && i < 8)
      std::cout << normalColor << "┠───────┼───────┼───────┨" << std::endl;
    if (i == 8)
      std::cout << normalColor << "┗━━━━━━━┷━━━━━━━┷━━━━━━━┛" << normalColor << std::endl;
  }
}
