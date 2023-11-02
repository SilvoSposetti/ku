#pragma once

#include "Board.h"
#include "constraints/AbstractConstraint.h"
#include "randomGenerator/RandomGenerator.h"

#include <memory>

namespace Setter {

/** Generates a random sudoku with the given inputs
 * @param totalDigits How many total digits it should have
 * @param symmetryType The pattern that should be used to create the given mask
 * @param constraints The set of constraints that are used to generate the solution
 * @param seed The seed for the random number generator used when setting the Sudoku
 * @return A randomly generated board
 */
std::unique_ptr<Board> generate(int32_t totalDigits,
                                const std::vector<std::unique_ptr<AbstractConstraint>>& constraints,
                                std::optional<int32_t> seed);

/** Generates a random sudoku with the given inputs
 * @param clues The explicitly-set given digits
 * @param constraints The set of constraints that are used to generate the solution
 * @param seed The seed for the random number generator used when setting the Sudoku
 * @return A randomly generated board
 */
std::unique_ptr<Board> generate(const std::vector<std::vector<Sudo::Digit>>& clues,
                                const std::vector<std::unique_ptr<AbstractConstraint>>& constraints,
                                std::optional<int32_t> seed);

/** Computes and returns a randomly generated mask
 * @param totalDigits How many digits should be left on the board
 * @param randomGenerator The random number generator instance used to draw random numbers
 * @return The constructed given mask
 */
std::vector<std::vector<bool>> randomMask(int32_t totalDigits, std::shared_ptr<RandomGenerator> randomGenerator);
}; // namespace Setter
