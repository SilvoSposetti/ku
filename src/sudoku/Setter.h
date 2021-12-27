#pragma once

#include "Board.h"
#include "../utilities/Utilities.h"
#include "constraints/AbstractConstraint.h"
#include <memory>

class Setter {
public:
    Setter() = delete;

    static std::unique_ptr<Board> generate(const int8_t totalDigits, const SetterType setterType, const std::vector<std::unique_ptr<AbstractConstraint>>& constraints);

private:
    static std::vector<std::vector<Sudo>> createRandomBoard(const std::vector<std::unique_ptr<AbstractConstraint>>& constraints);

    static bool createRandomRecursive(int8_t rowIndex,
                                      int8_t columnIndex,
                                      std::vector<std::vector<Sudo>>& board,
                                      const std::vector<std::unique_ptr<AbstractConstraint>>& constraints);

    static std::vector<std::vector<bool>> randomGivenMask(const int8_t totalDigits);
};



