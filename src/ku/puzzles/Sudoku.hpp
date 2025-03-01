#pragma once

#include "Puzzle.hpp"

/** Class for classic Sudoku
 */
class Sudoku : public Puzzle<9, 9, 9> {
public:
  virtual void printGrid() const override;
};