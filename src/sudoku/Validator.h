#pragma once

#include "../utilities/Utilities.h"
#include "Board.h"
#include "constraints/AbstractConstraint.h"
/** Class that holds static methods used to check whether a board satisfies all the constraints
 */
class Validator {
public:
  Validator() = delete;
  /** Checks if the board provided is a valid solution according to the given constraints
   */
  static bool checkSolution(const std::vector<std::vector<Sudo>>& board,
                            const std::vector<std::unique_ptr<AbstractConstraint>>& constraints);
};
