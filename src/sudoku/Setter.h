#pragma once

#include "Board.h"
#include "../utilities/Utilities.h"
#include "constraints/AbstractConstraint.h"
#include <memory>

class Setter {
public:
    Setter() = delete;

    static std::unique_ptr<Board> generate(const u_int8_t totalDigits, const SetterType setterType, const std::vector<std::unique_ptr<AbstractConstraint>>& constraints);
};



