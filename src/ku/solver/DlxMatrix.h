#pragma once

#include "SparseCoordinateMatrix.h"

#include <string>
#include <unordered_set>
#include <vector>

#define INVALID_INDEX -1
#define INVALID_DATA -1
#define INVALID_LENGTH -1

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
  int32_t data = INVALID_DATA;
  /// Pointer to left node
  int32_t left = INVALID_INDEX;
  /// Pointer to left node
  int32_t right = INVALID_INDEX;
  int32_t up = INVALID_INDEX;
  int32_t down = INVALID_INDEX;
  int32_t header = INVALID_INDEX;
  int32_t length = INVALID_LENGTH;

  std::string name;
};

class DlxMatrix {

public:
  DlxMatrix(const SparseCoordinateMatrix& sparseMatrix);

  // Returns a list of the options (rows) indices of the sparse matrix
  std::unordered_set<int32_t> solve();

private:
  // if successful, returns a list of the indices of the first nodes of the options that are part of the solution
  std::unordered_set<int32_t> runAlgorithmX();
  int32_t pickFirstSmallestColumnIndex() const;

  void cover(int32_t itemIndex);
  void uncover(int32_t itemIndex);
  void hide(int32_t optionIndex);
  void unhide(int32_t optionIndex);

private:
  int32_t itemsAmount;
  int32_t optionsAmount;

  std::vector<DlxNode> nodesList;
};