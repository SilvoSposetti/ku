#include "AlgorithmX.h"

#include "../randomGenerator/RandomGenerator.h"
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

  constexpr int32_t rootNodesAmount = 1; // One root
  const int32_t headerNodesAmount = itemsAmount; // As many header as there are items
  const int32_t regularNodesAmount = validElements; // One regular node for each non-zero elements in the matrix
  const int32_t spacerNodesAmount = optionsAmount + 1; // One spacer in front of each option + one spacer at the end
  const int32_t totalNodesAmount = rootNodesAmount + headerNodesAmount + regularNodesAmount + spacerNodesAmount;
  std::vector<AlgorithmXNode> structure(totalNodesAmount);

  // Set the root node
  structure[0].type = AlgorithmXNodeType::Root;

  // I1: Read the first line
  int32_t N_1 = -1;
  int32_t i = 0;

  for (int itemId = 0; itemId < itemsAmount; itemId++) {
    i++;
    structure[i].type = AlgorithmXNodeType::Header;
    // structure[i].name = std::to_string(itemId);
    structure[i].left = i - 1;
    structure[i - 1].right = i;
    if (!sparseMatrix.isColumnPrimary(itemId) && N_1 <= 0) {
      N_1 = i - 1;
    }
  }

  // I2:  Finish the horizontal list
  int32_t N = i;
  if (N_1 <= 0) {
    // There were no secondary items
    N_1 = N;
  }
  // The active secondary items (if any) are accessible from the node at index N+1
  structure[N + 1].left = N;
  structure[N].right = N + 1;
  structure[N_1 + 1].left = N + 1;
  structure[N + 1].right = N_1 + 1;
  structure[0].left = N_1;
  structure[N_1].right = 0;

  // I3: Prepare for options
  for (int32_t j = 1; j <= N; j++) {
    structure[j].length = 0;
    structure[j].up = j;
    structure[j].down = j;
  }
  int32_t M = 0;
  int32_t p = N + 1;
  // At this moment, p is the first spacer
  structure[p].type = AlgorithmXNodeType::Spacer;
  structure[p].header = -1;

  for (int32_t optionId = 0; optionId < optionsAmount; optionId++) {
    int32_t j = 0;
    for (const auto& i_j_index : sparseMatrix.getRowElements(optionId)) {
      const int32_t i_j = i_j_index + 1;
      j++;
      // I4: Read an option
      structure[p + j].type = AlgorithmXNodeType::Node;
      structure[i_j].length++;
      // p + j is the index of the current node, just added to the structure
      const int32_t q = structure[i_j].up; // q is the bottom-est node under header i_j
      structure[q].down = p + j; // Set the down of q to the current
      structure[p + j].up = q; // Set q as the top node of the current
      structure[p + j].down = i_j; // Set the down of the current node, which is now the bottom-est
      structure[p + j].header = i_j; // Set the header of the current node
      structure[i_j].up = p + j; // Set the up of the header to the
    }
    // I5: Finish an option
    const int32_t k = j;
    M = M + 1;
    structure[p].down = p + k;
    p = p + k + 1;
    structure[p].type = AlgorithmXNodeType::Spacer;
    structure[p].header = -M;
    structure[p].up = p - k;
  }

  return structure;
};

/** Computes the index of the item with smallest length in the active list of the data structure. If there are
 * multiple elements with the same smallest length, it returns the first one.
 * @param structure A reference to the structure currently being used
 * @return The index to the first smallest item in the data structure
 */
int32_t pickUsingMrvHeuristic(const std::vector<AlgorithmXNode>& structure, RandomGenerator& randomGenerator) {
  // This follows the MRV (Minimum Remaining Values) heuristic.
  int32_t maxLength = std::numeric_limits<int32_t>::max(); // Theta in Knuth's book, see answer to Exercise 9.
  int32_t index = -1;
  int32_t header = structure[0].right; // p in Knuth's book, see answer to Exercise 9.

  int32_t r = -1;
  while (header != 0) {
    const int32_t length = structure[header].length; // Lambda in Knuth's book, see answer to Exercise 9.
    if (length < maxLength) {
      index = header;
      maxLength = length;
      r = 1;
    }
    if (length == maxLength) {
      r++;
      if (randomGenerator.uniformFloat(0.0f, 1.0f) < 1.0f / static_cast<float>(r)) {
        index = header;
      }
    }

    header = structure[header].right;
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
    if (structure[nextNode].type == AlgorithmXNodeType::Spacer) { // Q is a spacer
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
    if (structure[previousNode].type == AlgorithmXNodeType::Spacer) { // Q is a spacer
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
std::vector<std::unordered_set<int32_t>> runAlgorithmX(const SparseCoordinateMatrix& sparseMatrix,
                                                       bool findFirstSolution,
                                                       bool checkForUniqueness,
                                                       const std::optional<int32_t>& seed) {

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

  // Initialize random generator to pick columns randomly according to a certain seed
  RandomGenerator randomGenerator(seed);

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
  itemIndex = pickUsingMrvHeuristic(structure, randomGenerator);
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
    if (structure[nextNodeIndex].type == AlgorithmXNodeType::Spacer) { // nextNode is a spacer
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
    if (structure[nodeIndex].type == AlgorithmXNodeType::Spacer) { // current node is a spacer
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

std::vector<std::unordered_set<int32_t>> AlgorithmX::findAllSolutions(const SparseCoordinateMatrix& sparseMatrix,
                                                                      const std::optional<int32_t>& seed) {
  // Algorithm x returns a list of node indices. These are the first nodes of the options which describe the solution
  return runAlgorithmX(sparseMatrix, false, false, seed);
}

std::unordered_set<int32_t> AlgorithmX::findOneSolution(const SparseCoordinateMatrix& sparseMatrix,
                                                        const std::optional<int32_t>& seed) {
  // Algorithm x returns a list of node indices. These are the first nodes of the options which describe the solution
  const auto solutions = runAlgorithmX(sparseMatrix, true, false, seed);
  if (solutions.size() >= 1) {
    return solutions[0];
  }
  return {};
}

bool AlgorithmX::hasUniqueSolution(const SparseCoordinateMatrix& sparseMatrix, const std::optional<int32_t>& seed) {
  return runAlgorithmX(sparseMatrix, false, true, seed).size() == 1;
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
    std::cout << s << std::endl;
    counter++;
  }
  std::cout << std::endl;
}