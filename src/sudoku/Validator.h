#pragma once

#include "Board.h"
#include "../utilities/Utilities.h"
#include "constraints/AbstractConstraint.h"
#include <memory>

class Validator {
public:
    Validator() = delete;

    static bool checkSolution(const std::vector<std::vector<Sudo>>& board,
                              const std::vector<std::unique_ptr<AbstractConstraint>>& constraints);

    static bool validateMove(const int8_t rowIndex,
                             const int8_t columnIndex,
                             const Sudo digit,
                             const std::vector<std::vector<Sudo>>& board,
                             const std::vector<std::unique_ptr<AbstractConstraint>>& constraints);

    static bool isBoardValid(const std::vector<std::vector<Sudo>>& board,
                             const std::vector<std::unique_ptr<AbstractConstraint>>& constraints);
};
