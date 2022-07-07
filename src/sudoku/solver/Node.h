#pragma once

#include "../../utilities/Utilities.h"

/** Node class used to implement the data structure for DLX (both the nodes and the headers)
 */
class Node {

public:
  /** Move constructor
   */
  explicit Node(std::string nodeName)
      : name(std::move(nodeName)){};
  /** Constructor
   */
  Node(int32_t row, int32_t column)
      : matrixRow(row)
      , matrixColumn(column){};

  /// Whether the header is primary
  bool isPrimary = true;
  /// How many elements the header (column) contains
  int32_t size = 0;
  /// Name of the header
  std::string name;
  /// The DLX-matrix row index of the node
  int32_t matrixRow = -1;
  /// The DLX-matrix column index of the node
  int32_t matrixColumn = -1;
  /// A pointer to the node on the left
  std::shared_ptr<Node> left;
  /// A pointer to the node on the right
  std::shared_ptr<Node> right;
  /// A pointer to the node upwards
  std::shared_ptr<Node> up;
  /// A pointer to the node downwards
  std::shared_ptr<Node> down;
  /// A pointer to the node's header
  std::shared_ptr<Node> header;
};
