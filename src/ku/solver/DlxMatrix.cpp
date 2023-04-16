#include "DlxMatrix.h"

#include <algorithm>
#include <iostream>
#include <limits>

DlxMatrix::DlxMatrix(const SparseCoordinateMatrix& sparseMatrix)
    : itemsAmount(sparseMatrix.getColumnsAmount())
    , optionsAmount(sparseMatrix.getRowsAmount()) {
  if (sparseMatrix.getValidElementsAmount() > 0) {
    createDlxDataStructure(sparseMatrix);
  }
}

std::unordered_set<int32_t> DlxMatrix::solve() {
  if (optionsAmount == 0 || itemsAmount == 0) {
    return {};
  }

  // Algorithm x returns a list of node indices. These are the first nodes of the options which describe the solution
  std::unordered_set<int32_t> nodeIndices = runAlgorithmX();

  // Find out which options are the solution
  std::unordered_set<int32_t> optionIds;
  for (const auto& id : nodeIndices) {
    // Count how many spacers are in front of the node
    const int32_t spacersAmount =
        std::count_if(structure.begin(), structure.begin() + id, [&](const DlxNode& node) -> bool {
          return node.type == DlxNodeType::Spacer;
        });
    optionIds.insert(spacersAmount - 1);
  }
  return optionIds;
}

void DlxMatrix::printDataStructure() const {
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
}

void DlxMatrix::createDlxDataStructure(const SparseCoordinateMatrix& sparseMatrix) {
  // DlxNodes contain "pointers" to other elements in the structure in the form of integer indices.
  // The structure is composed of multiple sections packed into a single vector of DlxNodes.
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

  // Create root
  DlxNode root{DlxNodeType::Root};
  structure.emplace_back(root);

  // Create headers
  for (int32_t i = 0; i < itemsAmount; i++) {
    const int32_t headerId = i + 1; // The root exists already
    structure.emplace_back(DlxNode{DlxNodeType::Header});
    structure[headerId].left = (headerId - 1) % (itemsAmount + 1);
    structure[headerId].right = (headerId + 1) % (itemsAmount + 1);
    structure[headerId].length = 0;
  };

  // make header list circular
  structure.front().right = 1;
  structure.front().left = structure.size() - 1;

  // Add the first spacer node
  structure.emplace_back(DlxNode{DlxNodeType::Spacer});

  // Add regular nodes
  for (int32_t i = 0; i < optionsAmount; i++) {
    for (int32_t j = 0; j < itemsAmount; j++) {
      const int32_t cellValue = sparseMatrix.getCell(i, j);
      if (cellValue >= 0) {
        const int32_t nodeId = structure.size();
        const int32_t headerId = j + 1;
        structure.emplace_back(DlxNode{DlxNodeType::Node});
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
    structure.emplace_back(DlxNode{DlxNodeType::Spacer});
  }

  // Spacer nodes are missing their up and down pointers.
  int32_t lastSpacerId = -1;
  for (size_t nodeId = 0; nodeId < structure.size(); nodeId++) {
    if (structure[nodeId].type == DlxNodeType::Spacer) {
      // If X is a spacer, X.up is the address of the first node in the option before x
      // If X is a spacer, X.down is the address of the last node in the option after x
      if (lastSpacerId > 0) {
        structure[nodeId].up = lastSpacerId + 1;
        structure[lastSpacerId].down = nodeId - 1;
      }
      lastSpacerId = nodeId;
    }
  }
}

std::unordered_set<int32_t> DlxMatrix::runAlgorithmX() {
  // x is the list of all options currently selected, this is used for backtracking
  // an option in this case is represented by the index of a node that is contained by the option
  std::vector<int32_t> x(optionsAmount, -1);

  // The following vector will store the solution, i.e. the list of nodes which are contained by the options that solve
  // the exact cover problem
  std::unordered_set<int32_t> solution = {};

  // The following algorithm is taken by the draft of Volume 4 of "The Art of Computer Programming" written by Donald
  // Knuth. In particular, this has been taken from the pre-fascicle 5c, which concentrates on the "Dancing Links"
  // technique.
  // It make use of goto-statements and the x vector as a means to perform backtracking. I've left the names of the
  // steps as Knuth names them (X1 to X8).

  // X1 (Initialization)
  int32_t level = 0; // Keeps track of the current "depth" level
  int32_t itemIndex = 0;
  int32_t nodeIndex = 0;
  int32_t otherItemIndex = 0;

X2: // Enter the current level
  std::cout << level << " | ";
  if (structure[0].right == 0) {
    solution = {x.begin(), x.begin() + level};
    goto X8;
  }
  // X3: // Choose an item
  itemIndex = pickFirstSmallestItemIndex();
  // X4: // Cover the item
  coverItem(itemIndex);
  x[level] = structure[itemIndex].down;
X5: // Try x[level]
  if (x[level] == itemIndex) {
    // Tried all options for the chosen item, need to backtrack
    goto X7;
  } else {
    int32_t nextNodeIndex = x[level] + 1;
    while (nextNodeIndex != x[level]) {
      int32_t nextNodeHeader = structure[nextNodeIndex].header;
      if (nextNodeHeader < 0) { // nextNode is a spacer
        // Follow nextNode's up to get the first node of the previous option
        nextNodeIndex = structure[nextNodeIndex].up;
      } else {
        // Cover items != itemIndex in the option that contains x[level]
        coverItem(nextNodeHeader);
        nextNodeIndex++;
      }
    }
    // Increase the level (do another "recursive" step)
    level++;
    goto X2;
  }
X6: // Try again
  nodeIndex = x[level] - 1;
  while (nodeIndex != x[level]) {
    otherItemIndex = structure[nodeIndex].header;
    if (otherItemIndex < 0) { // current node is a spacer
      // Follow current node's down to get the last node of the following option
      nodeIndex = structure[nodeIndex].down;
    } else {
      // Uncover the other item index
      uncoverItem(otherItemIndex);
      nodeIndex--;
    }
  }
  itemIndex = structure[x[level]].header;
  x[level] = structure[x[level]].down;
  goto X5;
X7: // Backtrack
  uncoverItem(itemIndex);
X8: // Leave level l
  if (level == 0) {
    goto END;
  } else {
    // Decrease level (backtrack one "recursive" step)
    level--;
    goto X6;
  }

END:
  return solution;
}

int32_t DlxMatrix::pickFirstSmallestItemIndex() const {
  // This follows the MRV (Minimum Remaining Values) heuristic.
  return structure[0].right;
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
}

void DlxMatrix::coverItem(int32_t itemIndex) {

  // Hide all options that contain a node for the item
  int32_t optionIndex = structure[itemIndex].down;
  while (optionIndex != itemIndex) {
    hideOption(optionIndex);
    optionIndex = structure[optionIndex].down;
  }
  // "Unlink" the header, but note that its links are preserved
  int32_t l = structure[itemIndex].left;
  int32_t r = structure[itemIndex].right;
  structure[l].right = r;
  structure[r].left = l;
}

void DlxMatrix::uncoverItem(int32_t itemIndex) {
  // "Re-link" the header by using its un-modified links
  int32_t l = structure[itemIndex].left;
  int32_t r = structure[itemIndex].right;
  structure[l].right = itemIndex;
  structure[r].left = itemIndex;

  // Un-hide all options that contained the item
  int32_t optionIndex = structure[itemIndex].up;
  while (optionIndex != itemIndex) {
    unhideOption(optionIndex);
    optionIndex = structure[optionIndex].up;
  }
}

void DlxMatrix::hideOption(int32_t optionNode) {
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
}

void DlxMatrix::unhideOption(int32_t optionNode) {
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
}