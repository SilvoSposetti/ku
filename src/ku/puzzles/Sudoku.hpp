#pragma once

#include "Puzzle.hpp"

/** Class for classic Sudoku
 */
class Sudoku : public Puzzle<9, 9, 9> {
public:
  Sudoku(const std::string& name,
         const std::vector<Cell>& clues,
         ConstraintType constraintTypes,
         std::optional<int32_t> seed)
      : Puzzle(name, clues, constraintTypes, seed) {}
};