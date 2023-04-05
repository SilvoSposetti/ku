#pragma once

#include "SparseCoordinateMatrix.h"

#include <string>
#include <vector>

enum class DlxNodeType : uint8_t { Root, Header, Node, Spacer };

struct DlxNode {

  DlxNode(DlxNodeType type)
      : type(type) {
    switch (type) {
    case DlxNodeType::Root:
      name = "root ";
      break;
    case DlxNodeType::Header:
      name = "head ";
      break;
    case DlxNodeType::Node:
      name = "node ";
      break;
    case DlxNodeType::Spacer:
      name = "space";
      break;
    }
  };

  DlxNodeType type;
  /// The data stored in the node. Used by nodes only
  int32_t data = -1;
  /// Pointer to left node
  int32_t left = 0;
  /// Pointer to left node
  int32_t right = 0;
  int32_t up = 0;
  int32_t down = 0;
  int32_t header = 0;
  int32_t length = 0;

  std::string name;
};

class DlxMatrix {

public:
  DlxMatrix(const SparseCoordinateMatrix& sparseMatrix);

  int32_t getFirstSmallestColumnIndex() const;

  void coverColumn(int32_t columnIndex);
  void uncoverColumn(int32_t columnIndex);

private:
  void cover(int32_t itemIndex);
  void uncover(int32_t itemIndex);
  void hide(int32_t optionIndex);
  void unhide(int32_t optionIndex);

private:
  int32_t itemsAmount;
  int32_t optionsAmount;

  std::vector<DlxNode> nodesList;
};