#pragma once

#include "Sudo.h"
#include "constraints/AbstractConstraint.h"

#include <memory>
#include <vector>

/** Class that holds static methods used to check whether a board satisfies all the constraints
 */
namespace Validator {
/** Checks if the board provided is a valid solution according to the given constraints
 */
bool checkSolution(const std::vector<std::vector<Sudo::Digit>>& board,
                   const std::vector<std::unique_ptr<AbstractConstraint>>& constraints);
}; // namespace Validator
