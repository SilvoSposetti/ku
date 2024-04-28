#include "AlgorithmX.h"

#include "../randomGenerator/RandomGenerator.h"

#include <algorithm>
#include <limits>

/** Helper functions for Algorithm X have been put here in an unnamed namespace such that they are only accessible by
 * this translation unit and nowhere else
 */
namespace {

/** Computes the index of the item with smallest length in the active list of the data structure. If there are
 * multiple elements with the same smallest length, it returns the first one.
 * @param structure A reference to the structure currently being used
 * @return The index to the first smallest item in the data structure
 */
int32_t pickUsingMrvHeuristic(const std::vector<Node>& structure, RandomGenerator& randomGenerator) {
  // This follows the MRV (Minimum Remaining Values) heuristic.
  int32_t minimumLength = std::numeric_limits<int32_t>::max();
  int32_t node = structure[0].right;
  int32_t selectedNode = -1;
  int32_t minimumLengthNodesAmount = 0;

  while (node != 0) {
    int32_t length = structure[node].length;
    if (length < minimumLength) {
      minimumLength = length;
      selectedNode = node;
      minimumLengthNodesAmount = 1;
    } else if (length == minimumLength) {
      minimumLengthNodesAmount++;
      if (randomGenerator.uniformFloat(0.0f, 1.0f) < (1.0f / static_cast<float>(minimumLengthNodesAmount))) {
        selectedNode = node;
      }
    }
    node = structure[node].right;
    if (minimumLength == 0) {
      break;
    }
  }
  return selectedNode;
};

/** Hides an option in the structure. Hiding means modifying node pointers such that the option disappears from the
 * active options.
 * @param structure A reference to the structure currently being used. This gets modified by this function
 * @param optionIndex The index of a node that is part of the option to be hidden
 */
void hideOption(std::vector<Node>& structure, int32_t optionNode) {
  // Hide all nodes of the option that contains node p
  int32_t nextNode = optionNode + 1;
  while (nextNode != optionNode) {
    const int32_t x = structure[nextNode].header;
    const int32_t u = structure[nextNode].up;
    const int32_t d = structure[nextNode].down;
    if (structure[nextNode].type == NodeType::Spacer) { // Q is a spacer
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
void unhideOption(std::vector<Node>& structure, int32_t optionNode) {
  // Unhide all nodes of the option that contains p
  int32_t previousNode = optionNode - 1;
  while (previousNode != optionNode) {
    const int32_t x = structure[previousNode].header;
    const int32_t u = structure[previousNode].up;
    const int32_t d = structure[previousNode].down;
    if (structure[previousNode].type == NodeType::Spacer) { // Q is a spacer
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
void coverItem(std::vector<Node>& structure, int32_t itemIndex) {
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
void uncoverItem(std::vector<Node>& structure, int32_t itemIndex) {
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

/** Computes sets of option indices (structure's rows) from sets of node indices in the structure
 * @param structure The structure that was used by Algorithm X
 * @param nodeIndicesSets The sets of node indices that describe Algorithm X's solutions
 */
std::vector<std::unordered_set<int32_t>>
retrieveOptionIndices(const std::vector<Node>& structure,
                      const std::vector<std::unordered_set<int32_t>>& nodeIndicesSets) {
  // Find out which options indices are the ones that the solution describes
  std::vector<std::unordered_set<int32_t>> optionIdsVector;
  for (const auto& nodeIndices : nodeIndicesSets) {
    std::unordered_set<int32_t> optionIds;
    for (const auto& nodeId : nodeIndices) {
      // Count how many spacers are in front of the node
      const int32_t spacersAmount =
          std::count_if(structure.begin(), structure.begin() + nodeId, [&](const Node& node) -> bool {
            return node.type == NodeType::Spacer;
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
 * @param dataStructure The sparse matrix that defines an Exact Cover problem
 * @return One or multiple sets of indices pointing to nodes in the data structure. The elements of these sets point
 * to the first nodes of options that are part of the solution found.
 */
std::vector<std::unordered_set<int32_t>> runAlgorithmX(const DataStructure& dataStructure,
                                                       bool findFirstSolution,
                                                       bool checkForUniqueness,
                                                       const std::optional<int32_t>& seed) {

  // Get a copy of the structure to work on
  std::vector<Node> structure = dataStructure.getStructureCopy();

  // Don't run Algorithm X on an empty structure
  if (structure.empty()) {
    return {};
  };
  const int32_t optionsAmount = dataStructure.getOptionsAmount();

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

X2: // Enter the current level
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
X5: // Try x[level]
{
  if (x[level] == itemIndex) {
    // Tried all options for the chosen item, need to backtrack
    goto X7;
  }
  int32_t nextNodeIndex = x[level] + 1;
  while (nextNodeIndex != x[level]) {
    int32_t nextNodeHeaderIndex = structure[nextNodeIndex].header;
    if (structure[nextNodeIndex].type == NodeType::Spacer) { // nextNode is a spacer
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

X6: // Try again
{
  nodeIndex = x[level] - 1;
  while (nodeIndex != x[level]) {
    otherItemIndex = structure[nodeIndex].header;
    if (structure[nodeIndex].type == NodeType::Spacer) { // current node is a spacer
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

X7: // Backtrack
{
  uncoverItem(structure, itemIndex);
}

X8: // Leave level l
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

END: { return retrieveOptionIndices(structure, solutions); }
};

} // namespace

std::vector<std::unordered_set<int32_t>> AlgorithmX::findAllSolutions(const DataStructure& dataStructure,
                                                                      const std::optional<int32_t>& seed) {
  // Algorithm x returns a list of node indices. These are the first nodes of the options which describe the solution
  return runAlgorithmX(dataStructure, false, false, seed);
}

std::unordered_set<int32_t> AlgorithmX::findOneSolution(const DataStructure& dataStructure,
                                                        const std::optional<int32_t>& seed) {
  // Algorithm x returns a list of node indices. These are the first nodes of the options which describe the solution
  const auto solutions = runAlgorithmX(dataStructure, true, false, seed);
  if (solutions.size() >= 1) {
    return solutions[0];
  }
  return {};
}

bool AlgorithmX::hasUniqueSolution(const DataStructure& dataStructure, const std::optional<int32_t>& seed) {
  return runAlgorithmX(dataStructure, false, true, seed).size() == 1;
}
