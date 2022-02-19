#include "Board.h"

Board::Board(const std::vector<std::vector<Sudo>>& inSolution, const std::vector<std::vector<bool>>& inGivenMask) :
        solution(inSolution),
        givenMask(inGivenMask) {

    // Copy solution to the field, then remove the non-given digits from it
    field = std::vector<std::vector<Sudo>>(solution);
    for (int8_t i = 0; i < MAX_DIGIT; i++) {
        for (int8_t j = 0; j < MAX_DIGIT; j++) {
            if (!givenMask[i][j]) {
                field[i][j] = Sudo::NONE;
            }
        }
    }
}


void Board::printGivenPattern() const {
    for (const auto& row: givenMask) {
        for (const auto& element: row) {
            const std::string str = element ? "■" : " ";
            std::cout << str << " ";
        }
        std::cout << std::endl;
    }
}

void Board::print() const {
    // Colors:
    const char normalColor[] = {0x1b, '[', '0', ';', '3', '9', 'm', 0};
    const char solvedColor[] = {0x1b, '[', '0', ';', '3', '7', 'm', 0};

    for (const auto& i: INDICES) {
        if (i == 0) {
            std::cout << normalColor << "┏━━━━━━━┯━━━━━━━┯━━━━━━━┓" << std::endl;
        }
        for (const auto& j: INDICES) {
            if (j == 0) {
                std::cout << normalColor + std::string("┃ ");
            }

            std::string c;
            if (givenMask[i][j]) c = normalColor + std::to_string(static_cast<int8_t>(solution[i][j]));
            else if (field[i][j] != Sudo::NONE) c = solvedColor + std::to_string(static_cast<int8_t>(field[i][j]));
            else c = solvedColor + std::string("◌");
            std::cout << c;

            if (j < 8) {
                if (j % 3 == 2) std::cout << normalColor << " │ ";
                else std::cout << normalColor << " ";
            }
            if (j == 8) std::cout << normalColor + std::string(" ┃");
        }
        std::cout << normalColor << std::endl;
        if (i % 3 == 2 && i < 8) std::cout << normalColor << "┠───────┼───────┼───────┨" << std::endl;
        if (i == 8) std::cout << normalColor << "┗━━━━━━━┷━━━━━━━┷━━━━━━━┛" << normalColor << std::endl;
    }
}

