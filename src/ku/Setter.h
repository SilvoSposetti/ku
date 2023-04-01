#pragma once

#include "Board.h"
#include "constraints/AbstractConstraint.h"
#include "randomGenerator/RandomGenerator.h"
#include "utilities/SymmetryType.h"

#include <memory>

namespace Setter {

/** Generates a random sudoku with the given inputs
 * @param totalDigits How many total digits it should have
 * @param symmetryType The pattern that should be used to create the given mask
 * @param constraints The set of constraints that are used to generate the solution
 * @param randomGenerator The random number generator instance used to draw random numbers
 * @return A randomly generated board
 */
std::unique_ptr<Board> generate(int32_t totalDigits,
                                SymmetryType symmetryType,
                                const std::vector<std::unique_ptr<AbstractConstraint>>& constraints,
                                std::shared_ptr<RandomGenerator> randomGenerator);
}; // namespace Setter
