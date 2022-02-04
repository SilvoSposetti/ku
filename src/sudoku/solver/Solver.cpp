#include "Solver.h"
#include "../Validator.h"
#include "Node.h"
#include <cstdint>
#include <memory>

std::vector<std::vector<Sudo>>
Solver::createBoard(const std::vector<std::unique_ptr<AbstractConstraint>>& constraints, SolverType solverType) {
    std::vector<std::vector<Sudo>> newBoard = emptyField();
    std::vector<std::vector<bool>> givenMask = emptyGivenMask();


    newBoard[0][1] = Sudo::G;
    givenMask[0][1] = true;
    newBoard[0][3] = Sudo::D;
    givenMask[0][3] = true;
    newBoard[1][0] = Sudo::C;
    givenMask[1][0] = true;
    newBoard[1][1] = Sudo::F;
    givenMask[1][1] = true;
    newBoard[1][2] = Sudo::E;
    givenMask[1][2] = true;
    newBoard[1][5] = Sudo::A;
    givenMask[1][5] = true;
    newBoard[2][0] = Sudo::H;
    givenMask[2][0] = true;
    newBoard[2][3] = Sudo::F;
    givenMask[2][3] = true;
    newBoard[2][5] = Sudo::G;
    givenMask[2][5] = true;
    newBoard[2][7] = Sudo::I;
    givenMask[2][7] = true;
    newBoard[3][0] = Sudo::I;
    givenMask[3][0] = true;
    newBoard[3][5] = Sudo::B;
    givenMask[3][5] = true;
    newBoard[3][6] = Sudo::D;
    givenMask[3][6] = true;
    newBoard[4][0] = Sudo::F;
    givenMask[4][0] = true;
    newBoard[4][3] = Sudo::E;
    givenMask[4][3] = true;
    newBoard[4][5] = Sudo::I;
    givenMask[4][5] = true;
    newBoard[4][8] = Sudo::H;
    givenMask[4][8] = true;
    newBoard[5][2] = Sudo::H;
    givenMask[5][2] = true;
    newBoard[5][3] = Sudo::A;
    givenMask[5][3] = true;
    newBoard[5][8] = Sudo::I;
    givenMask[5][8] = true;
    newBoard[6][1] = Sudo::C;
    givenMask[6][1] = true;
    newBoard[6][3] = Sudo::B;
    givenMask[6][3] = true;
    newBoard[6][5] = Sudo::H;
    givenMask[6][5] = true;
    newBoard[6][8] = Sudo::G;
    givenMask[6][8] = true;
    newBoard[7][3] = Sudo::G;
    givenMask[7][3] = true;
    newBoard[7][6] = Sudo::H;
    givenMask[7][6] = true;
    newBoard[7][7] = Sudo::C;
    givenMask[7][7] = true;
    newBoard[7][8] = Sudo::D;
    givenMask[7][8] = true;
    newBoard[8][5] = Sudo::C;
    givenMask[8][5] = true;
    newBoard[8][7] = Sudo::A;
    givenMask[8][7] = true;


    bool created = false;
    if (solverType == SolverType::BruteForce) {
        created = Solver::randomBruteForceRecursive(0, 0, newBoard, givenMask, constraints);
    } else if (solverType == SolverType::DLX) {
        created = Solver::randomDlx(newBoard, givenMask, constraints);
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


bool Solver::randomDlx(std::vector<std::vector<Sudo>>& board,
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
    const std::shared_ptr<Node> root = createDancingLinksMatrix(M, constraints);
    // printDancingLinksMatrix(root, constraints, board, givenMask);

    // Find possible solutions
    std::vector<std::vector<std::shared_ptr<Node>>> solutions = searchDlx(root);

    if (solutions.size() == 1) {
        std::cout << "Unique solution!" << std::endl;
        for(const auto& node: solutions.at(0)) {
            std::cout << std::to_string(node->matrixRow) << " | ";
        }
        std::cout << std::endl;
    }
    else{
        std::cout << "Found at least " << solutions.size() << " solutions: "<< std::endl;
        int32_t counter = 0;
        for(const auto& solution: solutions) {
            std::cout << counter << ": ";
            for(const auto& node: solution) {
                std::cout << std::to_string(node->matrixRow) << " | ";
            }
            std::cout << std::endl;
        }
    }
    
    


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
                // Go to the last existing node of the column, starting from the column header
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


std::vector<std::vector<std::shared_ptr<Node>>> Solver::searchDlx(const std::shared_ptr<Node>& root) {
    int32_t maxSolutions = 2;
    std::vector<std::vector<std::shared_ptr<Node>>> solutions;
    std::vector<std::shared_ptr<Node>> solutionHolder;

    searchDlxRecursive(root, 0, maxSolutions, solutionHolder, solutions);
    return solutions;
}

void Solver::searchDlxRecursive(const std::shared_ptr<Node>& root,
                                int32_t depth,
                                int32_t& solutionsLeftToSearchFor,
                                std::vector<std::shared_ptr<Node>> solutionHolder,
                                std::vector<std::vector<std::shared_ptr<Node>>>& solutions) {

    if (root->right == root) {
        // Ran out of columns => solved the exact cover problem

        solutions.emplace_back(solutionHolder);
        solutionsLeftToSearchFor--;
        return;
    }
    
    // Pick a column to cover
    std::shared_ptr<Node> currentColum = root->right;
    // std::shared_ptr<Node> currentColum = chooseSmallestColumn(root);
    // std::shared_ptr<Node> currentColum = chooseRandomColumn(root);

    coverDlxColumn(currentColum);

    std::shared_ptr<Node> node = currentColum->down;
    // Go through all nodes of this colum and cover their siblings' columns

    // Stop if we reach loop back to the current column's header, or if enough solutions have been found
    while (node != currentColum && solutionsLeftToSearchFor > 0) {
        // Cover all siblings of this column's node one by one
        // This is emplaced at the element with index 'depth'
        solutionHolder.push_back(node);
        std::shared_ptr<Node> siblingNode = node->right;

        // Cover all siblings of 'node' from left to right
        while (siblingNode != node) {
            coverDlxColumn(siblingNode->header);
            siblingNode = siblingNode->right;
        }

        // Recursive call
        searchDlxRecursive(root, depth + 1, solutionsLeftToSearchFor, solutionHolder, solutions);        

        // A solution wasn't found, pop the node from the solution
        node = solutionHolder.at(depth);
        solutionHolder.pop_back();
        currentColum = node->header;

        // Uncover the node's siblings in the opposite order, from right to left
        siblingNode = node->left;
        while (siblingNode != node) {
            uncoverDlxColumn(siblingNode->header);
            siblingNode =  siblingNode->left;
        }

        // Proceed down to next node of this column
        node = node->down;

    }
    uncoverDlxColumn(currentColum);

}

void Solver::coverDlxColumn(std::shared_ptr<Node>& column) {
    // Important: Even thought the column header is unlinked from the matrix,
    // note that here column->left and column->right are not modified 
    column->right->left = column->left;
    column->left->right = column->right;

    std::shared_ptr<Node> node = column->down;
    while (node != column) {
        std::shared_ptr<Node> siblingNode = node->right;
        while (siblingNode != node){
            // Important: Even though the sibling nodes are unlinked from the matrix,
            // note that here siblingNode->up and siblingNode->down are not modified
            siblingNode->down->up = siblingNode->up;
            siblingNode->up->down = siblingNode->down;

            // Just unlinked a node from a colum => reduce  column size
            siblingNode->header->size--;

            // Proceed to next sibling (left to right)
            siblingNode = siblingNode->right;
        }
        // Proceed to next node down the coumn
        node = node->down;
    }
}

void Solver::uncoverDlxColumn(std::shared_ptr<Node>& column) {
    // Uncovering is done in reverse to covering
    std::shared_ptr<Node> node = column->up;
    while (node != column) {
        std::shared_ptr<Node> siblingNode = node->left;
        while (siblingNode != node) {
            // The sibling nodes' pointers were unmodified while covering,
            // insert the sibling node back into the list using the "Dancing Links" operation
            siblingNode->down->up = siblingNode;
            siblingNode->up->down = siblingNode;

            // Just re-inserted the node in the column => increase column size
            siblingNode->header->size++;

            // Proceed to next sibling (right to left)
            siblingNode = siblingNode->left;
        }
        // Proceed to the next node up the column
        node = node->up;
    }
    // The column header's pointers were unmodified while covering it,
    // insert the column header back into the list using the "Dancing Links" operation
    column->right->left = column;
    column->left->right = column;
}


std::shared_ptr<Node> Solver::chooseSmallestColumn(const std::shared_ptr<Node>& root) {
    std::shared_ptr<Node> currentColumn = root->right;
    std::shared_ptr<Node> smallestColumn = root->right;
    while (currentColumn != root){
        if (currentColumn->size < currentColumn->size){
            smallestColumn = currentColumn;
        }
        currentColumn = currentColumn->right;
    }
    return smallestColumn;
}

std::shared_ptr<Node> Solver::chooseRandomColumn(const std::shared_ptr<Node>& root) {
    std::shared_ptr<Node> currentColumn = root;
    int32_t columnsAmount = 0;
    while (currentColumn->right != root){
        columnsAmount++;
        currentColumn = currentColumn->right;
    }
    const int32_t randomColumn = randomUniform(0, columnsAmount);
    currentColumn = root->right;
    std::cout << randomColumn << std::endl;
    int32_t counter = 0;
    while (counter < randomColumn) {
        currentColumn =  currentColumn->right;
        counter++;
    }
    return currentColumn;
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

