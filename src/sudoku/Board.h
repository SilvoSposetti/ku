#pragma once

#include "../utilities/Utilities.h"

class Board {
public:
  Board(const std::vector<std::vector<Sudo>>& inSolution, const std::vector<std::vector<bool>>& inGivenMask);

  void printGivenPattern() const;

  void print() const;

private:
  std::vector<std::vector<Sudo>> field;
  const std::vector<std::vector<Sudo>> solution;
  const std::vector<std::vector<bool>> givenMask;
};
