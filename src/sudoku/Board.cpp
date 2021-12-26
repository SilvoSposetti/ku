#include "Board.h"
#include "../utilities/Logger.h"

Board::Board() {
    // Create empty solution
    for (uint8_t i = 0; i <= MAX_DIGITS; i++) {
        std::vector<Sudo> row(MAX_DIGITS + 1, Sudo::NONE);
        solution.emplace_back(row);
    }
    // Copy empty solution to the given and field
    given = std::vector<std::vector<Sudo>>(solution);
    field = std::vector<std::vector<Sudo>>(solution);
}

void Board::printField() {
    std::cout << "Field:" << std::endl;
    for (const auto& row: field) {
        for (const auto& digit: row) {
            std::cout << std::to_string(static_cast<uint8_t>(digit)) << "  ";
        }
        std::cout << std::endl;
    }
}

void Board::printSolution() {
    std::cout << "Solution:" << std::endl;
    for (const auto& row: solution) {
        for (const auto& digit: row) {
            std::cout << std::to_string(static_cast<uint8_t>(digit)) << "  ";
        }
        std::cout << std::endl;
    }
}

void Board::printGiven() {
    std::cout << "Given:" << std::endl;
    for (const auto& row: given) {
        for (const auto& digit: row) {
            std::cout << std::to_string(static_cast<uint8_t>(digit)) << "  ";
        }
        std::cout << std::endl;
    }
}


void Board::printAll() {
    printField();
    std::cout << std::endl;
    printSolution();
    std::cout << std::endl;
    printGiven();
}

