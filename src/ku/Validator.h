#pragma once

#include "Board.h"
#include "Sudo.h"
#include "constraints/AbstractConstraint.h"

#include <memory>

/** Class that holds static methods used to check whether a board satisfies all the constraints
 */
class Validator {
public:
  Validator() = delete;
  /** Checks if the board provided is a valid solution according to the given constraints
   */
  static bool checkSolution(const std::vector<std::vector<Sudo::Digit>>& board,
                            const std::vector<std::unique_ptr<AbstractConstraint>>& constraints);
};
