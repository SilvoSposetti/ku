#pragma once

#include "Puzzle.hpp"

/** Class for the 6x6 variant of classic Sudoku
 */
class Sudoku6x6 : public Puzzle<6, 6, 6> {
public:
  virtual void printGrid() const override;
};