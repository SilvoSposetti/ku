#include "DlxMatrix.h"

#include <iostream>

DlxMatrix::DlxMatrix(const SparseCoordinateMatrix& sparseMatrix)
    : itemsAmount(sparseMatrix.getColumnsAmount())
    , optionsAmount(sparseMatrix.getRowsAmount()) {

  // Create root
  DlxNode root{DlxNodeType::Root};
  nodesList.emplace_back(root);

  // Create headers
  for (int32_t i = 0; i < itemsAmount; i++) {
    const int32_t headerId = i + 1; // The root exists already
    nodesList.emplace_back(DlxNode{DlxNodeType::Header});
    nodesList[headerId].left = (headerId - 1) % (itemsAmount + 1);
    nodesList[headerId].right = (headerId + 1) % (itemsAmount + 1);
    nodesList[headerId].length = 0;
  };

  // make header list circular
  nodesList.front().right = 1;
  nodesList.front().left = nodesList.size() - 1;

  // Add a spacer node
  nodesList.emplace_back(DlxNode{DlxNodeType::Spacer});

  // Add regular nodes
  for (int32_t i = 0; i < optionsAmount; i++) {
    for (int32_t j = 0; j < itemsAmount; j++) {
      const int32_t cellValue = sparseMatrix.getCell(i, j);
      if (cellValue >= 0) {
        const int32_t nodeId = nodesList.size();
        const int32_t headerId = j + 1;
        nodesList.emplace_back(DlxNode{DlxNodeType::Node});
        nodesList.back().header = headerId;
        nodesList.back().data = cellValue;

        // Increase header length since we just added a node
        nodesList[headerId].length++;
        if (nodesList[headerId].down <= 0) {
          nodesList[headerId].down = nodeId;
          nodesList[headerId].up = nodeId;
          nodesList[nodeId].up = headerId;
          nodesList[nodeId].down = headerId;
        } else {
          int32_t lastNodeId = nodesList[headerId].up;
          nodesList[lastNodeId].down = nodeId;
          nodesList[headerId].up = nodeId;
          nodesList[nodeId].down = headerId;
          nodesList[nodeId].up = lastNodeId;
        }
      }
    }
    nodesList.emplace_back(DlxNode{DlxNodeType::Spacer});
  }

  // Fill spacer nodes. They just need up & down.
  int32_t lastSpacerId = -1;
  for (size_t nodeId = 0; nodeId < nodesList.size(); nodeId++) {
    if (nodesList[nodeId].type == DlxNodeType::Spacer) {
      // x.up is the address of the first node in the option before x
      // x.down is the address of the last node in the option after x
      nodesList[nodeId].up = lastSpacerId + 1;
      if (lastSpacerId > 0) {
        nodesList[lastSpacerId].down = nodeId - 1;
      }
      lastSpacerId = nodeId;
    }
  }

  const auto getInfo = [&](const std::string& inName, int32_t value) -> std::string {
    if (value > 0) {
      std::string valueString = std::to_string(value);
      while (valueString.size() < 2) {
        valueString = " " + valueString;
      }
      return inName + ": " + valueString + " | ";
    }
    return "";
  };

  int32_t counter = 0;
  for (const auto& node : nodesList) {
    std::string s;
    s += getInfo("ID", counter);
    s += node.name + " \t";
    s += getInfo("L", node.left);
    s += getInfo("R", node.right);
    s += getInfo("U", node.up);
    s += getInfo("D", node.down);
    s += getInfo("H", node.header);
    s += getInfo("LEN", node.length);
    s += getInfo("Data", node.data);
    std::cout << s << std::endl;
    counter++;
  }
}

int32_t DlxMatrix::getFirstSmallestColumnIndex() const {
  return 0; // TODO: implement
}

void DlxMatrix::coverColumn(int32_t columnIndex) {
  cover(columnIndex + 1);
}

void DlxMatrix::uncoverColumn(int32_t columnIndex) {
  uncover(columnIndex + 1);
}

void DlxMatrix::cover(int32_t itemIndex) {

  // Hide all options that contain
  int32_t p = nodesList[itemIndex].down;
  while (p != itemIndex) {
    hide(p);
    p = nodesList[p].down;
  }
  // "Unlink" the header, but note that its links are preserved
  int32_t l = nodesList[itemIndex].left;
  int32_t r = nodesList[itemIndex].right;
  nodesList[l].right = r;
  nodesList[r].left = l;
}

void DlxMatrix::uncover(int32_t itemIndex) {
  // "Re-link" the header
  int32_t l = nodesList[itemIndex].left;
  int32_t r = nodesList[itemIndex].right;
  nodesList[l].right = itemIndex;
  nodesList[r].left = itemIndex;

  int32_t p = nodesList[itemIndex].up;
  while (p != itemIndex) {
    unhide(p);
    p = nodesList[p].up;
  }
}

void DlxMatrix::hide(int32_t p) {

  int32_t q = p + 1;
  while (q != p) {
    const int32_t x = nodesList[q].header;
    const int32_t u = nodesList[q].up;
    const int32_t d = nodesList[q].down;
    if (x <= 0) { // Q is a spacer
      q = u;
    } else {
      nodesList[u].down = d;
      nodesList[d].up = u;
      nodesList[x].length--;
      q++;
    }
  }
}

void DlxMatrix::unhide(int32_t p) {
  int32_t q = p - 1;
  while (q != p) {
    const int32_t x = nodesList[q].header;
    const int32_t u = nodesList[q].up;
    const int32_t d = nodesList[q].down;
    if (x <= 0) {
      q = d;
    } else {
      nodesList[u].down = q;
      nodesList[d].up = q;
      nodesList[x].length++;
      q--;
    }
  }
}