#include "Solver.h"
#include "../Validator.h"
#include <cstdint>

std::vector<std::vector<Sudo>>
Solver::createNewBoard(const std::vector<std::unique_ptr<AbstractConstraint>>& constraints) {
    
    std::vector<std::vector<Sudo>> newField = emptyField();
    std::vector<std::vector<bool>> givenMask = emptyGivenMask();
    
    // Naive solver is faster to create a filled, random board
    bool created = Solver::naive(newField, givenMask, constraints);

    if (!created) {
        std::cout << "ERROR: Was not able to generate a new board with the given constraints" << std::endl;
    }
    if (!Validator::checkSolution(newField, constraints)) {
        std::cout << "ERROR: Solution created does not satisfy all constraints" << std::endl;
    }
    return newField;
}

bool Solver::isUnique(const std::vector<std::vector<Sudo>>& solution,
                      const std::vector<std::vector<bool>>& givenMask,
                      const std::vector<std::unique_ptr<AbstractConstraint>> &constraints) {
    
    // Create board to use for solving
    std::vector<std::vector<Sudo>> board = solution;
    for(int i = 0; i < MAX_INDEX; i++){
        for(int j = 0; j < MAX_INDEX; j++){
            if(!givenMask[i][j]){
                board[i][j] = Sudo::NONE;
            }
        }
    }
    // Run DLX
    return Solver::dlx(board, givenMask, constraints, true);
}

bool Solver::naive(std::vector<std::vector<Sudo>>& board,
                   const std::vector<std::vector<bool>>& givenMask,
                   const std::vector<std::unique_ptr<AbstractConstraint>>& constraints) {
                       int32_t counter = 0;
    return naiveRecursive(0, 0, board, givenMask, constraints);
}

bool Solver::naiveRecursive(int8_t rowIndex,
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
                if (Solver::naiveRecursive(rowIndex, columnIndex, board, givenMask, constraints)) {
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
        return Solver::naiveRecursive(rowIndex, columnIndex, board, givenMask, constraints);
    }
    return false;
}


bool Solver::dlx(std::vector<std::vector<Sudo>>& board,
                 const std::vector<std::vector<bool>>& givenMask,
                 const std::vector<std::unique_ptr<AbstractConstraint>>& constraints,
                 bool checkForUniqueness) {
    // Reduce problem: Sudoku->DLX
    
    // Create matrix
    const int32_t totalRows = 9 * 9 * 9; // =729 (81 cells, 9 possible digits for each cell)
    int32_t totalColumns = 0;
    for (const auto& constraint: constraints) {
        totalColumns += constraint->getDLXConstraintColumnsAmount();
    }

    std::vector<std::vector<int32_t>> M; // stores the cell ID if a node exists in a certain position, otherwise -1

    // Go through all board cells via their row and column indices
    for (const auto& boardRow : INDICES) {
        for (const auto& boardColumn : INDICES) {
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
    std::vector<std::vector<std::shared_ptr<Node>>> solutions = searchDlx(root, checkForUniqueness);
 
    // No need to reduce the solution back to a valid board when simply checking for uniqueness
    if(checkForUniqueness){
        if(solutions.size() == 1){
            return true;
        }
        return false;
    }

    // Reduce solution: DLX->Sudoku
    if (!solutions.empty()) {        
        // Use first solution out of all those that are found
        const std::vector<std::shared_ptr<Node>> pickedSolution = solutions[0];
        // This solution should have 81 nodes, one for each cell
        if (pickedSolution.size() == TOTAL_DIGITS) {
            // Transform found solution to the sudoku board
            for(const auto& node : pickedSolution) { // Pick the first solution
                // The node itself stores its original DLX-matrix location (row & column)
                const int32_t matrixRow = node->matrixRow;
                // This uses the same method used to identify the cells of the SUDOKU_CELL constraint,
                // but the process here is reversed
                const int32_t boardRow = matrixRow / TOTAL_DIGITS ;
                const int32_t boardColumn = (matrixRow / MAX_DIGIT) % MAX_DIGIT;
                const Sudo actualDigit = static_cast<Sudo>((matrixRow % MAX_DIGIT) + 1);
                board[boardRow][boardColumn] = actualDigit;
            }
            return true;
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


std::vector<std::vector<std::shared_ptr<Node>>> Solver::searchDlx(const std::shared_ptr<Node>& root,
                                                                  bool checkForUniqueness) {
    // The solution holder is modified by the search, everytime it reaches 81 elements a different solution
    // is found by the algorithm
    std::vector<std::shared_ptr<Node>> solutionHolder;
    // The solutions are going to contain all different 81-sized solutions found
    std::vector<std::vector<std::shared_ptr<Node>>> solutions;
    // Stop after this amount of solutions is found
    int32_t maxSolutions = checkForUniqueness ? 2 : 1;

    searchDlxRecursive(root, 0, maxSolutions, solutionHolder, solutions);
    return solutions;
}

void Solver::searchDlxRecursive(const std::shared_ptr<Node>& root,
                                int32_t depth,
                                int32_t& solutionsLeftToSearchFor,
                                std::vector<std::shared_ptr<Node>>& solutionHolder,
                                std::vector<std::vector<std::shared_ptr<Node>>>& solutions) {

    if (root->right == root) {
        // Ran out of columns => solved the exact cover problem
        solutions.emplace_back(solutionHolder);
        solutionsLeftToSearchFor--;
        return;
    }
    // Pick a column to cover
    // std::shared_ptr<Node> currentColum = root->right;
    std::shared_ptr<Node> currentColum = chooseSmallestColumn(root);
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
        if (currentColumn->size < smallestColumn->size){
            smallestColumn = currentColumn;
        }
        currentColumn = currentColumn->right;
    }
    return smallestColumn;
}

std::shared_ptr<Node> Solver::chooseRandomColumn(const std::shared_ptr<Node>& root) {
    std::shared_ptr<Node> currentColumn = root->right;
    int32_t columnsAmount = 0;
    while (currentColumn != root){
        columnsAmount++;
        currentColumn = currentColumn->right;
    }
    const int32_t randomColumn = randomUniform(1, columnsAmount);

    currentColumn = root;
    int32_t counter = 0;
    while (counter < randomColumn) {
        currentColumn = currentColumn->right;
        counter++;
    }
    return currentColumn;
}

void Solver::printDancingLinksMatrix(const std::shared_ptr<Node>& root,
                                     const std::vector<std::unique_ptr<AbstractConstraint>>& constraints,
                                     const std::vector<std::vector<Sudo>>& board,
                                     const std::vector<std::vector<bool>>& givenMask) {
    
    // The amount of rows depends on the amount of givens, each given removes 8 rows
    constexpr int32_t maximumRows = 9 * 9 * 9;
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

