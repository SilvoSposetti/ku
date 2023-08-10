#include "AlgorithmX.h"

#include "AlgorithmXNode.h"

#include <algorithm>
#include <iostream>
#include <limits>

/** Helper functions for Algorithm X have been put here in an unnamed namespace such that they are only accessible by
 * this translation unit and nowhere else
 */
namespace {

/** Constructs the data structure that will be used internally by Algorithm X.
 * @param sparseMatrix The sparse matrix used for construction
 * @return The data structure of nodes
 */
std::vector<AlgorithmXNode> createDataStructure(const SparseCoordinateMatrix& sparseMatrix) {
  int32_t itemsAmount = sparseMatrix.getColumnsAmount();
  int32_t optionsAmount = sparseMatrix.getRowsAmount();
  int32_t validElements = sparseMatrix.getValidElementsAmount();
  if (itemsAmount == 0 || optionsAmount == 0 || validElements == 0) {
    return {};
  }

  // AlgorithmXNodes contain "pointers" to other elements in the structure in the form of integer indices.
  // The structure is composed of multiple sections packed into a single vector of AlgorithmXNodes.
  // 1. A root node.
  // 2. A list of header nodes. A header is given to every item.
  //    - Header nodes make a circular doubly linked-list together with the root node.
  //    - Header nodes have a length attribute that keeps track of how many elements are contained within the item.
  // 3. A list of all the valid spare coordinate matrix entries in row-major order. Different rows are separated by
  //    spacer nodes. These spacer nodes appear at both ends of these entries as well.
  //    And additionally:
  //    - Every node of the sparse coordinate matrix composes a 2-dimensional doubly-linked list because it holds
  //    pointers to its left-right- and to its up-down-neighbors in the original sparse matrix.
  //    - Every node also holds a pointer to its header.
  // In particular, a spacer node X has:
  // - An up-link to the first node in the option before X.
  // - An down-link to the last node in the option after X.

  // TODO: allocate the right amount of memory here using validElements
  std::vector<AlgorithmXNode> structure;

  // Create root
  AlgorithmXNode root{AlgorithmXNodeType::Root};
  structure.emplace_back(root);

  // Create headers
  for (int32_t i = 0; i < itemsAmount; i++) {
    const int32_t headerId = i + 1; // The root exists already
    structure.emplace_back(AlgorithmXNode{AlgorithmXNodeType::Header});
    structure[headerId].left = (headerId - 1) % (itemsAmount + 1);
    structure[headerId].right = (headerId + 1) % (itemsAmount + 1);
    structure[headerId].length = 0;
  };

  // make header list circular
  structure.front().right = 1;
  structure.front().left = structure.size() - 1;

  // Unlink all secondary column headers
  for (int i = 1; i < itemsAmount + 1; i++) {
    if (!sparseMatrix.isColumnPrimary(i - 1)) {
      // Unlink from header list
      structure[structure[i].right].left = structure[i].left;
      structure[structure[i].left].right = structure[i].right;
      // Set left and right pointers to to itself
      structure[i].left = i;
      structure[i].right = i;
    }
  }

  // Add the first spacer node
  structure.emplace_back(AlgorithmXNode{AlgorithmXNodeType::Spacer});

  // Add regular nodes
  for (int32_t i = 0; i < optionsAmount; i++) {
    for (int32_t j = 0; j < itemsAmount; j++) {
      const int32_t cellValue = sparseMatrix.getCell(i, j);
      if (cellValue >= 0) {
        const int32_t nodeId = structure.size();
        const int32_t headerId = j + 1;
        structure.emplace_back(AlgorithmXNode{AlgorithmXNodeType::Node});
        structure.back().header = headerId;
        structure.back().data = cellValue;

        // Increase header length since we just added a node
        structure[headerId].length++;
        if (structure[headerId].down <= 0) {
          structure[headerId].down = nodeId;
          structure[headerId].up = nodeId;
          structure[nodeId].up = headerId;
          structure[nodeId].down = headerId;
        } else {
          int32_t lastNodeId = structure[headerId].up;
          structure[lastNodeId].down = nodeId;
          structure[headerId].up = nodeId;
          structure[nodeId].down = headerId;
          structure[nodeId].up = lastNodeId;
        }
      }
    }
    // Add a spacer node after all nodes of the option have been added
    structure.emplace_back(AlgorithmXNode{AlgorithmXNodeType::Spacer});
  }

  // Spacer nodes are missing their up and down pointers.
  int32_t lastSpacerId = -1;
  for (size_t nodeId = 0; nodeId < structure.size(); nodeId++) {
    if (structure[nodeId].type == AlgorithmXNodeType::Spacer) {
      // If X is a spacer, X.up is the address of the first node in the option before x
      // If X is a spacer, X.down is the address of the last node in the option after x
      if (lastSpacerId > 0) {
        structure[nodeId].up = lastSpacerId + 1;
        structure[lastSpacerId].down = nodeId - 1;
      }
      lastSpacerId = nodeId;
    }
  }
  return structure;
};

/** Computes the index of the item with smallest length in the active list of the data structure. If there are
 * multiple elements with the same smallest length, it returns the first one.
 * @param structure A reference to the structure currently being used
 * @return The index to the first smallest item in the data structure
 */
int32_t pickFirstSmallestItemIndex(const std::vector<AlgorithmXNode>& structure) {
  // This follows the MRV (Minimum Remaining Values) heuristic.
  int32_t maxLength = std::numeric_limits<int32_t>::max();
  int32_t index = -1;
  int32_t column = structure[0].right;
  while (column != 0) {
    const int32_t length = structure[column].length;
    if (length < maxLength) {
      index = column;
      maxLength = length;
    }
    column = structure[column].right;
  }
  if (maxLength != std::numeric_limits<int32_t>::max()) {
    return index;
  }
  return -1;
};

/** Hides an option in the structure. Hiding means modifying node pointers such that the option disappears from the
 * active options.
 * @param structure A reference to the structure currently being used. This gets modified by this function
 * @param optionIndex The index of a node that is part of the option to be hidden
 */
void hideOption(std::vector<AlgorithmXNode>& structure, int32_t optionNode) {
  // Hide all nodes of the option that contains node p
  int32_t nextNode = optionNode + 1;
  while (nextNode != optionNode) {
    const int32_t x = structure[nextNode].header;
    const int32_t u = structure[nextNode].up;
    const int32_t d = structure[nextNode].down;
    if (x < 0) { // Q is a spacer
      nextNode = u; // up link of a spacer points to the first node of the previous option
    } else {
      structure[u].down = d;
      structure[d].up = u;
      structure[x].length--;
      nextNode++;
    }
  }
};

/** Unhides an option in the structure. Unhiding means modifying node pointers such that the option reappears from the
 available options.
 * @param structure A reference to the structure currently being used. This gets modified by this function
 * @param optionIndex The index of a node that is part of the option to be unhidden
 */
void unhideOption(std::vector<AlgorithmXNode>& structure, int32_t optionNode) {
  // Unhide all nodes of the option that contains p
  int32_t previousNode = optionNode - 1;
  while (previousNode != optionNode) {
    const int32_t x = structure[previousNode].header;
    const int32_t u = structure[previousNode].up;
    const int32_t d = structure[previousNode].down;
    if (x < 0) { // Q is a spacer
      previousNode = d; // down link of a spacer points to the last node of the following option
    } else {
      structure[u].down = previousNode;
      structure[d].up = previousNode;
      structure[x].length++;
      previousNode--;
    }
  }
};

/** Covers an item in the structure. Covering means modifying node pointers such that the item does not appear in the
 * active list.
 * @param structure A reference to the structure currently being used. This gets modified by this function
 * @param itemIndex The index of the item that needs to be covered
 */
void coverItem(std::vector<AlgorithmXNode>& structure, int32_t itemIndex) {
  // Hide all options that contain a node for the item
  int32_t optionIndex = structure[itemIndex].down;
  while (optionIndex != itemIndex) {
    hideOption(structure, optionIndex);
    optionIndex = structure[optionIndex].down;
  }
  // "Unlink" the header, but note that its links are preserved
  int32_t l = structure[itemIndex].left;
  int32_t r = structure[itemIndex].right;
  structure[l].right = r;
  structure[r].left = l;
};

/** Uncovers an item in the structure. Uncovering means modifying node pointers such that the item reappears in the
 * active list.
 * @param structure A reference to the structure currently being used. This gets modified by this function
 * @param itemIndex The index of the item that needs to be uncovered
 */
void uncoverItem(std::vector<AlgorithmXNode>& structure, int32_t itemIndex) {
  // "Re-link" the header by using its un-modified links
  int32_t l = structure[itemIndex].left;
  int32_t r = structure[itemIndex].right;
  structure[l].right = itemIndex;
  structure[r].left = itemIndex;

  // Un-hide all options that contained the item
  int32_t optionIndex = structure[itemIndex].up;
  while (optionIndex != itemIndex) {
    unhideOption(structure, optionIndex);
    optionIndex = structure[optionIndex].up;
  }
};

/** Computes sets of option indices (SparseMatrix's rows) from sets of node indices in the structure
 * @param structure The structure that was used by Algorithm X
 * @param nodeIndicesSets The sets of node indices that describe Algorithm X's solutions
 */
std::vector<std::unordered_set<int32_t>>
retrieveOptionIndices(const std::vector<AlgorithmXNode>& structure,
                      const std::vector<std::unordered_set<int32_t>>& nodeIndicesSets) {
  // Find out which options indices are the ones that the solution describes
  std::vector<std::unordered_set<int32_t>> optionIdsVector;
  for (const auto& nodeIndices : nodeIndicesSets) {
    std::unordered_set<int32_t> optionIds;
    for (const auto& nodeId : nodeIndices) {
      // Count how many spacers are in front of the node
      const int32_t spacersAmount =
          std::count_if(structure.begin(), structure.begin() + nodeId, [&](const AlgorithmXNode& node) -> bool {
            return node.type == AlgorithmXNodeType::Spacer;
          });
      optionIds.insert(spacersAmount - 1);
    }
    optionIdsVector.emplace_back(optionIds);
  }
  return optionIdsVector;
};

/** Runs Algorithm X on the internal data structure to find a solution.
 * @param findFirstSolution Whether a single solution suffices. When true this offers early exit as soon as the first
 * one is found
 * @param checkForUniqueness Whether confirming a unique solution is required. When true this offers early exit
 * if more than one solution is found.
 * @param sparseMatrix The sparse matrix that defines an Exact Cover problem
 * @return One or multiple sets of indices pointing to nodes in the data structure. The elements of these sets point
 * to the first nodes of options that are part of the solution found.
 */
std::vector<std::unordered_set<int32_t>>
runAlgorithmX(const SparseCoordinateMatrix& sparseMatrix, bool findFirstSolution, bool checkForUniqueness) {

  // Don't run Algorithm X on an empty sparse matrix
  std::vector<AlgorithmXNode> structure = createDataStructure(sparseMatrix);
  if (structure.empty()) {
    return {};
  };
  const int32_t optionsAmount = sparseMatrix.getColumnsAmount();

  // x is the list of all options currently selected, this is used for backtracking
  // an option in this case is represented by the index of a node that is contained by the option
  std::vector<int32_t> x(optionsAmount, -1);

  // The following vector will store the solutions, i.e. the sets of nodes which are contained by the options that solve
  // the exact cover problem
  std::vector<std::unordered_set<int32_t>> solutions;

  // The following algorithm is taken by the draft of Volume 4 of "The Art of Computer Programming" written by Donald
  // Knuth. In particular, this has been taken from the pre-fascicle 5c, which concentrates on the "Dancing Links"
  // technique.
  // It make use of goto-statements and the x vector as a means to perform backtracking. I've left the names of the
  // steps as Knuth names them (Step X1 to Step X8).

  // X1 // Initialize, structure is already set up
  int32_t level = 0; // Keeps track of the current "depth" level
  int32_t itemIndex = 0;
  int32_t nodeIndex = 0;
  int32_t otherItemIndex = 0;

X2 : // Enter the current level
{
  if (structure[0].right == 0) {
    // The list of currently selected options x contains a valid solution! Store it.
    solutions.emplace_back(std::unordered_set<int32_t>({x.begin(), x.begin() + level}));
    goto X8;
  }
  // X3: // Choose an item
  itemIndex = pickFirstSmallestItemIndex(structure);
  // X4: // Cover the item
  coverItem(structure, itemIndex);
  x[level] = structure[itemIndex].down;
}
X5 : // Try x[level]
{
  if (x[level] == itemIndex) {
    // Tried all options for the chosen item, need to backtrack
    goto X7;
  }
  int32_t nextNodeIndex = x[level] + 1;
  while (nextNodeIndex != x[level]) {
    int32_t nextNodeHeaderIndex = structure[nextNodeIndex].header;
    if (nextNodeHeaderIndex < 0) { // nextNode is a spacer
      // Follow nextNode's up to get the first node of the previous option
      nextNodeIndex = structure[nextNodeIndex].up;
    } else {
      // Cover items != itemIndex in the option that contains x[level]
      coverItem(structure, nextNodeHeaderIndex);
      nextNodeIndex++;
    }
  }
  // Increase the level (do another "recursive" step)
  level++;
  goto X2;
}

X6 : // Try again
{
  nodeIndex = x[level] - 1;
  while (nodeIndex != x[level]) {
    otherItemIndex = structure[nodeIndex].header;
    if (otherItemIndex < 0) { // current node is a spacer
      // Follow current node's down to get the last node of the following option
      nodeIndex = structure[nodeIndex].down;
    } else {
      // Uncover the other item index, doing the opposite as in X5
      uncoverItem(structure, otherItemIndex);
      nodeIndex--;
    }
  }
  itemIndex = structure[x[level]].header;
  x[level] = structure[x[level]].down;
  goto X5;
}

X7 : // Backtrack
{
  uncoverItem(structure, itemIndex);
}

X8 : // Leave level l
{
  // Exit early if checking for uniqueness and more than one solution is found
  const bool uniquenessEarlyExit = checkForUniqueness && solutions.size() >= 2;
  // Exit early if it's not required to find all solutions and at least one has been found
  const bool singleSolutionEarlyExit = findFirstSolution && solutions.size() >= 1;
  // When the level reaches 0, all possible solutions have been evaluated
  const bool searchedThroughAllPossibilities = level == 0;
  if (singleSolutionEarlyExit || uniquenessEarlyExit || searchedThroughAllPossibilities) {
    goto END;
  } else {
    // Decrease level (backtrack one "recursive" step)
    level--;
    goto X6;
  }
}

END : { return retrieveOptionIndices(structure, solutions); }
};

} // namespace

std::vector<std::unordered_set<int32_t>> AlgorithmX::findAllSolutions(const SparseCoordinateMatrix& sparseMatrix) {
  // Algorithm x returns a list of node indices. These are the first nodes of the options which describe the solution
  return runAlgorithmX(sparseMatrix, false, false);
}

std::unordered_set<int32_t> AlgorithmX::findOneSolution(const SparseCoordinateMatrix& sparseMatrix) {
  // Algorithm x returns a list of node indices. These are the first nodes of the options which describe the solution
  const auto solutions = runAlgorithmX(sparseMatrix, true, false);
  if (solutions.size() >= 1) {
    return solutions[0];
  }
  return {};
}

bool AlgorithmX::hasUniqueSolution(const SparseCoordinateMatrix& sparseMatrix) {
  return runAlgorithmX(sparseMatrix, false, true).size() == 1;
}

void AlgorithmX::printDataStructure(const SparseCoordinateMatrix& sparseMatrix) {
  const std::vector<AlgorithmXNode> structure = createDataStructure(sparseMatrix);

  // Helper function
  const auto getInfo = [&](const std::string& inName, int32_t value) -> std::string {
    if (value >= 0) {
      std::string valueString = std::to_string(value);
      while (valueString.size() < 2) {
        valueString = " " + valueString;
      }
      return inName + ": " + valueString + "   ";
    }
    return "";
  };

  int32_t counter = 0;
  for (const auto& node : structure) {
    std::string s;
    s += getInfo("ID", counter);
    s += node.getTypeString() + "  ";
    s += getInfo("H", node.header);
    s += getInfo("U", node.up);
    s += getInfo("D", node.down);
    s += getInfo("L", node.left);
    s += getInfo("R", node.right);
    s += getInfo("LEN", node.length);
    s += getInfo("Data", node.data);
    std::cout << s << std::endl;
    counter++;
  }
  std::cout << std::endl;
}