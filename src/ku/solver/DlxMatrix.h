#pragma once

#include "SparseCoordinateMatrix.h"

#include <string>
#include <unordered_set>
#include <vector>

enum class DlxNodeType : uint8_t { Root, Header, Node, Spacer };

struct DlxNode {

  DlxNode(DlxNodeType type)
      : type(type) {
    switch (type) {
    case DlxNodeType::Root:
      name = "root  ";
      break;
    case DlxNodeType::Header:
      name = "header";
      break;
    case DlxNodeType::Node:
      name = "node  ";
      break;
    case DlxNodeType::Spacer:
      name = "spacer";
      break;
    default:
      name = "node  ";
      break;
    }
  };

  DlxNodeType type = DlxNodeType::Node;
  /// The data stored in the node. Used by nodes only
  int32_t data = -1;
  /// Pointer to left node
  int32_t left = -1;
  /// Pointer to left node
  int32_t right = -1;
  int32_t up = -1;
  int32_t down = -1;
  int32_t header = -1;
  int32_t length = -1;

  std::string name;
};

/** Class that encapsulates the data structure used by Algorithm X and the functions that are run on it.
 * It is used to find solutions of Exact Cover problems described by a sparse matrix.
 */
class DlxMatrix {

public:
  /** Constructor
   * @param sparseMatrix The sparse matrix representing the Exact Cover problem
   */
  DlxMatrix(const SparseCoordinateMatrix& sparseMatrix);

  /** Solves the exact cover problem
   * @return Returns a set of option (row) indices that solve the Exact Cover problem
   */
  std::unordered_set<int32_t> solve();

  /** Logs to standard output the internal data structure used. Useful for debugging
   */
  void printDataStructure() const;

private:
  /** Constructs the internal data structure
   * @param sparseMatrix The sparse matrix used for construction
   */
  void createDlxDataStructure(const SparseCoordinateMatrix& sparseMatrix);

  /** Runs Algorithm X on the internal data structure to find a solution.
   * @return A set of indices pointing to nodes in the data structure. These point to the first nodes of options that
   * are part of the solution found.
   */
  std::unordered_set<int32_t> runAlgorithmX();

  /** Computes the index of the item with smallest length in the active list of the data structure. If there are
   * multiple elements with the same smallest length, it returns the first one.
   * @return The index to the first smallest item in the data structure
   */
  int32_t pickFirstSmallestItemIndex() const;

  /** Covers an item. Covering means modifying node pointers such that the item does not appear in the active list.
   * Modifies the internal data structure.
   * @param itemIndex The index of the item that needs to be covered
   */
  void coverItem(int32_t itemIndex);

  /** Uncovers an item. Uncovering means modifying node pointers such that the item reappears in the active
   * list. Modifies the internal data structure.
   * @param itemIndex The index of the item that needs to be uncovered
   */
  void uncoverItem(int32_t itemIndex);

  /** Hides an option. Hiding means modifying node pointers such that the option disappears from the available options.
   * Modifies the internal data structure.
   * @param optionIndex The index of a node that is part of the option to be hidden
   */
  void hideOption(int32_t optionNode);

  /** Unhides an option. Unhiding means modifying node pointers such that the option reappears from the available
   * options. Modifies the internal data structure.
   * @param optionIndex The index of a node that is part of the option to be unhidden
   */
  void unhideOption(int32_t optionNode);

private:
  /// The amount of items in the exact cover problem
  int32_t itemsAmount = 0;
  /// The amount of options in the exact cover problem
  int32_t optionsAmount = 0;

  /// The data structure. A list of nodes constructed from a sparse matrix
  std::vector<DlxNode> structure = {};
};