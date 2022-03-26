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
                      const std::vector<std::unique_ptr<AbstractConstraint>>& constraints) {

  // Create board to use for solving
  std::vector<std::vector<Sudo>> board = solution;
  for (const auto& i : INDICES) {
    for (const auto& j : INDICES) {
      if (!givenMask[i][j]) {
        board[i][j] = Sudo::NONE;
      }
    }
  }
  // Run DLX
  return Solver::dlx(board, constraints, true);
}

std::vector<std::vector<int32_t>>
Solver::getDlxMatrix(const std::vector<std::vector<Sudo>>& board,
                     const std::vector<std::unique_ptr<AbstractConstraint>>& constraints) {
  // To initialize the matrix with the correct size: count how many digits are given
  int32_t givenAmount = 0;
  for (const auto& row : board) {
    for (const auto& digit : row) {
      if (digit != Sudo::NONE) {
        ++givenAmount;
      }
    }
  }
  // 729 => (81 cells, 9 possible digits for each cell)
  constexpr int32_t maximumRows = 9 * 9 * 9;
  // Each given reduces the amount of rows by (MAX_DIGIT - 1)
  const int32_t totalRows = maximumRows - (MAX_DIGIT - 1) * givenAmount;

  int32_t totalColumns = 0;
  for (const auto& constraint : constraints) {
    totalColumns += constraint->getDLXConstraintColumnsAmount();
  }

  // Initialize matrix with correct size
  std::vector<std::vector<int32_t>> matrix(totalRows, std::vector<int32_t>(totalColumns, -1));

  int32_t matrixRowCounter = 0;
  int32_t matrixColumnCounter = 0;
  for (const auto& boardI : INDICES) { // Go through all sudoku rows
    for (const auto& boardJ : INDICES) { // Go through all sudoku column
      const Sudo actualDigit = board[boardI][boardJ];
      for (const auto& possibleDigit : SUDO_DIGITS) { // Go through all possible digits for this cell
        // Avoid rows where the cell and its possible digits don't create a "1" in the matrix, since the digit
        // is already given
        if (actualDigit == Sudo::NONE || actualDigit == possibleDigit) {
          for (const auto& constraint : constraints) {
            for (int32_t columnId = 0; columnId < constraint->getDLXConstraintColumnsAmount(); ++columnId) {
              if (constraint->getDLXConstraint(boardI, boardJ, columnId, possibleDigit, actualDigit)) {
                // Store matrix cell ID:
                matrix[matrixRowCounter][matrixColumnCounter] =
                    boardI * TOTAL_DIGITS + boardJ * MAX_DIGIT + (static_cast<int32_t>(possibleDigit) - 1);
              }
              matrixColumnCounter = (matrixColumnCounter + 1) % totalColumns;
            }
          }
          matrixRowCounter++;
        }
      }
    }
  }

  return matrix;
}

bool Solver::naive(std::vector<std::vector<Sudo>>& board,
                   const std::vector<std::vector<bool>>& givenMask,
                   const std::vector<std::unique_ptr<AbstractConstraint>>& constraints) {
  int32_t counter = 0;
  return naiveRecursive(0, 0, board, givenMask, constraints);
}

bool Solver::naiveRecursive(int32_t rowIndex,
                            int32_t columnIndex,
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
    for (const auto& digit : randomDigits) {
      // Check if the current digit can be placed here without breaking any constraint
      if (Validator::validateMove(rowIndex, columnIndex, digit, board, constraints)) {
        // Store digit's place
        const int32_t k = rowIndex;
        const int32_t l = columnIndex;
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
                 const std::vector<std::unique_ptr<AbstractConstraint>>& constraints,
                 bool checkForUniqueness) {
  // Reduce problem: Sudoku->DLX
  std::vector<std::vector<int32_t>> M = getDlxMatrix(board, constraints);

  // Create doubly-linked list according to matrix M
  const std::shared_ptr<Node> root = createDancingLinksMatrix(M, constraints);

  // Find possible solutions
  std::vector<std::vector<std::shared_ptr<Node>>> solutions = searchDlx(root, checkForUniqueness);

  // No need to reduce the solution back to a valid board when simply checking for uniqueness
  if (checkForUniqueness) {
    if (solutions.size() == 1) {
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
      for (const auto& node : pickedSolution) { // Pick the first solution
        // The node itself stores its original DLX-matrix location (row & column)
        const int32_t matrixRow = node->matrixRow;
        // This uses the same method used to identify the cells of the SUDOKU_CELL constraint,
        // but the process here is reversed
        const int32_t boardRow = matrixRow / TOTAL_DIGITS;
        const int32_t boardColumn = (matrixRow / MAX_DIGIT) % MAX_DIGIT;
        const Sudo actualDigit = static_cast<Sudo>((matrixRow % MAX_DIGIT) + 1);
        board[boardRow][boardColumn] = actualDigit;
      }
      return true;
    }
  }
  return false;
}

std::shared_ptr<Node>
Solver::createDancingLinksMatrix(const std::vector<std::vector<int32_t>>& matrix,
                                 const std::vector<std::unique_ptr<AbstractConstraint>>& constraints) {
  const int32_t totalRows = static_cast<int32_t>(matrix.size());
  const int32_t totalColumns = static_cast<int32_t>(matrix[0].size());

  std::shared_ptr<Node> root = std::make_shared<Node>("root");
  root->header = root;
  std::shared_ptr<Node> currentColumnHeader = root;

  // First create all column headers
  for (const auto& constraint : constraints) {
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
    node = solutionHolder[depth];
    solutionHolder.pop_back();
    currentColum = node->header;

    // Uncover the node's siblings in the opposite order, from right to left
    siblingNode = node->left;
    while (siblingNode != node) {
      uncoverDlxColumn(siblingNode->header);
      siblingNode = siblingNode->left;
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
    while (siblingNode != node) {
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
  while (currentColumn != root) {
    if (currentColumn->size < smallestColumn->size) {
      smallestColumn = currentColumn;
    }
    currentColumn = currentColumn->right;
  }
  return smallestColumn;
}