#include "SudokuColumn.h"

ConstraintType SudokuColumn::getType() {
    return ConstraintType::SUDOKU_BOX;
}

std::string SudokuColumn::getName() {
    return "Sudoku-Column";
}

std::string SudokuColumn::getDescription() {
    return "1x9 columns contain all the digits from 1 to 9 exactly once.";
}

std::string SudokuColumn::getSvgGroup() {
    return std::string();
}
