#include "Solver.h"
#include "../Validator.h"
#include "Node.h"
#include <cstdint>

std::vector<std::vector<Sudo>>
Solver::createBoard(const std::vector<std::unique_ptr<AbstractConstraint>>& constraints, const SolverType solverType) {
    std::vector<std::vector<Sudo>> newBoard = emptyField();
    std::vector<std::vector<bool>> givenMask = emptyGivenMask();

//    newBoard[0][0] = Sudo::C;
//    givenMask[0][0] = true;
//    newBoard[0][2] = Sudo::F;
//    givenMask[0][2] = true;
//    newBoard[8][5] = Sudo::H;
//    givenMask[8][5] = true;

    bool created = false;
    if (solverType == SolverType::BruteForce) {
        created = Solver::randomBruteForceRecursive(0, 0, newBoard, givenMask, constraints);
    } else if (solverType == SolverType::DLX) {
        created = Solver::randomDLX(newBoard, givenMask, constraints);
    }

    if (!created) {
        std::cout << "ERROR: Was not able to generate a new board with the given constraints" << std::endl;
    }
    if (!Validator::checkSolution(newBoard, constraints)) {
        std::cout << "ERROR: Solution created does not satisfy all constraints" << std::endl;
    }
    return newBoard;
}

bool Solver::randomBruteForceRecursive(int8_t rowIndex,
                                       int8_t columnIndex,
                                       std::vector<std::vector<Sudo>>& board,
                                       const std::vector<std::vector<bool>>& givenMask,
                                       const std::vector<std::unique_ptr<AbstractConstraint>>& constraints) {
    // Reached end of the board, so found solution must be valid
    if (rowIndex > 8) {
        return true;
    }
    // Current digit can be modified if it's not part of the given mask
    if (!givenMask[rowIndex][columnIndex]) {
        // Try out all possible digits in random order
        std::vector<Sudo> randomDigits = randomShuffle(SUDO_DIGITS);
        for (const auto& digit: randomDigits) {
            // Check if the current digit can be placed here without breaking any constraint
            if (Validator::validateMove(rowIndex, columnIndex, digit, board, constraints)) {
                // Store digit's place
                const int8_t k = rowIndex;
                const int8_t l = columnIndex;
                board[k][l] = digit; // Try and set the current digit in the board
                // Go forward in the grid
                columnIndex++;
                if (columnIndex > 8) {
                    rowIndex++;
                    columnIndex = 0;
                }
                // If the recursive call returns true, a valid solution was found
                if (Solver::randomBruteForceRecursive(rowIndex, columnIndex, board, givenMask, constraints)) {
                    return true;
                }
                // Otherwise, the digit just placed leads to a conflict
                board[k][l] = Sudo::NONE;
                // Thus, go back one digit in the board
                columnIndex--;
                if (columnIndex < 0) {
                    rowIndex--;
                    columnIndex = 8;
                }
            }
        }
    } else {
        // The digit cannot be modified, go to the next one
        columnIndex++;
        if (columnIndex > 8) {
            rowIndex++;
            columnIndex = 0;
        }
        return Solver::randomBruteForceRecursive(rowIndex, columnIndex, board, givenMask, constraints);
    }
    return false;
}


bool Solver::randomDLX(std::vector<std::vector<Sudo>>& board,
                       const std::vector<std::vector<bool>>& givenMask,
                       const std::vector<std::unique_ptr<AbstractConstraint>>& constraints) {
    // Create matrix
    const int32_t totalRows = 9 * 9 * 9; // =729 (81 cells, 9 possible digits for each cell)
    int32_t totalColumns = 0;
    for (const auto& constraint: constraints) {
        totalColumns += constraint->getDLXConstraintColumnsAmount();
    }

    std::vector<std::vector<int32_t>> M; // stores the cell ID if a node exists in a certain position, otherwise -1

    // Go through all board cells via their row and column indices
    for (int8_t boardRow = 0; boardRow <= MAX_INDEX; boardRow++) {
        for (int8_t boardColumn = 0; boardColumn <= MAX_INDEX; boardColumn++) {
            Sudo actualDigit = Sudo::NONE;
            if(givenMask[boardRow][boardColumn]) {
                actualDigit = board[boardRow][boardColumn];
            }
            // Go through all possible digits that could go in the cell
            for (const auto& possibleDigit: SUDO_DIGITS) {
                // Avoid rows where the cell and its possible digits don't create a "1" in the matrix, since the digit
                // is already given
                if (actualDigit == Sudo::NONE || actualDigit == possibleDigit) {
                    std::vector<int32_t> row;
                    for (const auto& constraint: constraints) {
                        // Let the constraint determine how many columns it needs for DLX, which will be returned to it indexed from 0
                        int32_t totalConstraintColumns = constraint->getDLXConstraintColumnsAmount();
                        for (int32_t columnId = 0; columnId < totalConstraintColumns; columnId++) {
                            // Retrieve whether the constraint prescribes a "1" for this row (cell & possibleDigit) and column(constraint) of the matrix
                            const bool exists = constraint->getDLXConstraint(boardRow,
                                                                            boardColumn,
                                                                            columnId,
                                                                            possibleDigit,
                                                                            board[boardRow][boardColumn]);
                            int32_t value = -1;
                            if (exists) {
                                // Store matrix cell ID: 
                                value = boardRow * TOTAL_DIGITS + boardColumn * MAX_DIGIT + (static_cast<int32_t>(possibleDigit) - 1); 
                            }
                            row.emplace_back(value);
                        }
                    }
                    M.emplace_back(row);
                }
            }
        }
    }
    // Create doubly-linked list according to matrix M
    std::shared_ptr<Node> root = createDancingLinksMatrix(M, constraints);
    printDancingLinksMatrix(root, constraints, board, givenMask);

    return false;
}

std::shared_ptr<Node> Solver::createDancingLinksMatrix(const std::vector<std::vector<int32_t>>& matrix,
                                                       const std::vector<std::unique_ptr<AbstractConstraint>>& constraints) {
    const int32_t totalRows = static_cast<int32_t>(matrix.size());
    const int32_t totalColumns = static_cast<int32_t>(matrix[0].size());

    std::shared_ptr<Node> root = std::make_shared<Node>("root");
    root->header = root;
    std::shared_ptr<Node> currentColumnHeader = root;

    // First create all column headers
    for (const auto& constraint: constraints) {
        for (int32_t columnId = 0; columnId < constraint->getDLXConstraintColumnsAmount(); columnId++) {
            const std::string columnName = constraint->getName() + "[colID: " + std::to_string(columnId) + "]";
            std::shared_ptr<Node> newHeader = std::make_shared<Node>(columnName);
            currentColumnHeader->right = newHeader;
            newHeader->left = currentColumnHeader;
            newHeader->header = newHeader; // Assign the header of a column to itself
            currentColumnHeader = newHeader;
        }
    }
    // Make column headers circular
    currentColumnHeader->right = root;
    root->left = currentColumnHeader;

    // Go through the entire matrix row-by-row
    for (int32_t rowIndex = 0; rowIndex < totalRows; rowIndex++) {
        currentColumnHeader = root->right;
        // Last one created for the current row, new nodes are placed on its right
        std::shared_ptr<Node> lastCreatedRowNode;
        // First one created for the current row, to which the last one must be connected
        std::shared_ptr<Node> firstRowNode;

        // Go through all columns
        for (int32_t columnIndex = 0; columnIndex < totalColumns; columnIndex++) {
            const int32_t matrixRowId = matrix[rowIndex][columnIndex];
            if (matrixRowId >= 0) {
                // Go down the column, starting from the column header
                std::shared_ptr<Node> lastColumnNode = currentColumnHeader;
                while (lastColumnNode->down) {
                    lastColumnNode = lastColumnNode->down;
                }
                // End of column reached, create new node
                std::shared_ptr<Node> newNode = std::make_shared<Node>(matrixRowId, columnIndex);
                // If it's the first one of the current row, store a reference to it
                if (!firstRowNode) {
                    firstRowNode = newNode;
                }
                // Set links and increment column header's counter
                lastColumnNode->down = newNode;
                newNode->up = lastColumnNode;
                newNode->header = currentColumnHeader;
                newNode->left = lastCreatedRowNode;
                if (lastCreatedRowNode) {
                    lastCreatedRowNode->right = newNode;
                }
                currentColumnHeader->size++;
                // Store the last created row node
                lastCreatedRowNode = newNode;
            }
            // Advance to next column
            currentColumnHeader = currentColumnHeader->right;
        }

        if (lastCreatedRowNode) {
            // Make row circular
            lastCreatedRowNode->right = firstRowNode;
            firstRowNode->left = lastCreatedRowNode;
        }
    }

    // Columns need to be circular
    currentColumnHeader = root->right;
    while (currentColumnHeader != root) {
        // Go down the column until columnEnd points to the last column's node
        std::shared_ptr<Node> columnEnd = currentColumnHeader;
        while (columnEnd->down) {
            columnEnd = columnEnd->down;
        }
        // Make the column circular
        columnEnd->down = currentColumnHeader;
        currentColumnHeader->up = columnEnd;
        // Advance to next column
        currentColumnHeader = currentColumnHeader->right;
    }
    return root;
}


void Solver::printDancingLinksMatrix(const std::shared_ptr<Node>& root,
                                     const std::vector<std::unique_ptr<AbstractConstraint>>& constraints,
                                     const std::vector<std::vector<Sudo>>& board,
                                     const std::vector<std::vector<bool>>& givenMask) {
    
    // The amount of rows depends on the amount of givens, each given removes 8 rows
    const int32_t maximumRows = 9 * 9 * 9;
    int32_t actualRows = maximumRows;
    int32_t givenDigits = 0;
    for (const auto& givenRow : givenMask) {
        for (const auto& element : givenRow) {
            if (element) {
                givenDigits++;;
            }
        }
    }
    actualRows -= givenDigits * 8;

    // The amount of columns depends on the amount of constraints
    int32_t columnsAmount = 0;
    for(const auto& constraint: constraints) {
        columnsAmount += constraint->getDLXConstraintColumnsAmount();
    }

    // Uncomment some of these lines to print only some specific constraint columns
    ConstraintType allowedConstraints = ConstraintType::NONE;
    allowedConstraints = allowedConstraints | ConstraintType::SUDOKU_CELL;
    allowedConstraints = allowedConstraints | ConstraintType::SUDOKU_ROW;
    allowedConstraints = allowedConstraints | ConstraintType::SUDOKU_COLUMN;
    allowedConstraints = allowedConstraints | ConstraintType::SUDOKU_BOX;
    std::vector<std::string> allowedConstraintNames;
    for(const auto& constraint: constraints) {
        if (static_cast<bool>(constraint->getType() & allowedConstraints)) {
           allowedConstraintNames.emplace_back(constraint->getName());
        }
    }

    // Print matrix header
    std::cout << "Dancing Links   |";
    for (const auto& constraint: constraints) {
        if(static_cast<bool>(constraint->getType() & allowedConstraints)) {
            std::string str = " " + constraint->getName();
            while (str.size() < constraint->getDLXConstraintColumnsAmount() - 1) {
                str += " ";
            }
            std::cout << str << "|";
        }
    }
    std::cout << std::endl;

    // Go through all rows
    for (int32_t rowIndex = 0; rowIndex < maximumRows; rowIndex++) {
        // Retrieve and print information about the row
        const int32_t boardRow = rowIndex / TOTAL_DIGITS ;
        const int32_t boardColumn = (rowIndex / MAX_DIGIT) % MAX_DIGIT;
        const Sudo possibleDigit = static_cast<Sudo>((rowIndex % MAX_DIGIT) + 1);

        const std::string boardRowString = std::to_string(boardRow);
        const std::string boardColumnString = std::to_string(boardColumn);
        const std::string possibleDigitString = std::to_string(static_cast<int8_t>(possibleDigit));
        
        // Print row header
        std::cout << "[" << boardRowString << ", " << boardColumnString
                    << "] Digit: " << possibleDigitString << " | ";

        // Go through all columns
        std::shared_ptr<Node> currentColumn = root->right;
        while (currentColumn != root) {
            // Show only columns that are allowed to be printed
            bool showColumn = false;
            for (const auto& constraintName : allowedConstraintNames){
                if(currentColumn->name.find(constraintName) != std::string::npos){
                    showColumn = true;
                }
            }

            if (showColumn) {
                // Look for a node at the row fo the currentColumn
                bool found = false;
                std::shared_ptr<Node> currentNode = currentColumn->down;
                while (currentNode != currentColumn) {
                    if (currentNode->matrixRow == rowIndex) {
                        found = true;

                    } else if (currentNode->matrixRow > rowIndex) {
                        break;
                    }
                    currentNode = currentNode->down;
                }
                if (found) {
                    std::cout << "■";
                } else {
                    std::cout << " ";
                }   
            }
            // Advance to next column
            currentColumn = currentColumn->right;
        }
        // Row has finished printing, go to next line to print the next row
        std::cout << std::endl;
    }
    // Print additional info:
    std::cout << "Matrix: " << actualRows << " rows, " << columnsAmount << " columns" << std::endl;
    std::cout << "Rows are "<< actualRows << " due to " << givenDigits
              << " given(s). (Max. 729 rows with 0 givens, each given removes 8 rows)" << std::endl;
    std::cout << columnsAmount << " columns due to " << constraints.size()
              << " constraints, namely [";
    for (int32_t i = 0; i < constraints.size(); i++) {
        std::cout << constraints[i]->getName() << ": " << constraints[i]->getDLXConstraintColumnsAmount();
        if (i != constraints.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;
}

