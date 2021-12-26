#include "SudokuBox.h"

ConstraintType SudokuBox::getType() {
    return ConstraintType::SUDOKU_BOX;
}

std::string SudokuBox::getName() {
    return "Sudoku-Box";
}

std::string SudokuBox::getDescription() {
    return "3x3 boxes contain all the digits from 1 to 9 exactly once.";
}

std::string SudokuBox::getSvgGroup() {
    return "";
}
