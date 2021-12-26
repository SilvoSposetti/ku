#pragma once

#include "../utilities/Utilities.h"
#include <vector>

class Board {
public:
    Board();

    void printField();
    void printSolution();
    void printGiven();
    void printAll();

private:
    void printRow(const std::vector<Sudo>& row);

    std::vector<std::vector<Sudo>> solution;
    std::vector<std::vector<Sudo>> given;
    std::vector<std::vector<Sudo>> field;
};
