#include "SudokuRow.h"

ConstraintType SudokuRow::getType() {
    return ConstraintType::SUDOKU_ROW;
}

std::string SudokuRow::getName() {
    return "Sudoku-Row";
}

std::string SudokuRow::getDescription() {
    return "9x1 rows contain all the digits from 1 to 9 exactly once.";
}

std::string SudokuRow::getSvgGroup() {
    return "";
}
