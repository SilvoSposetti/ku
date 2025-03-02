#pragma once

#include "Puzzle.hpp"

/** Class for the 6x6 variant of classic Sudoku
 */
class Sudoku6x6 : public Puzzle<6, 6, 6> {
public:
  Sudoku6x6(const std::string& name,
            const std::vector<Cell>& clues,
            ConstraintType constraintTypes,
            std::optional<int32_t> seed)
      : Puzzle(name, clues, constraintTypes, seed) {}

  virtual void printGrid() const override;
};