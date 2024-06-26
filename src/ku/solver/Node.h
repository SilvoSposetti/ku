#pragma once

#include <cstdint>
#include <string>

/** Enum class defining the type of a node
 */
enum class NodeType : uint8_t { Root, Header, Node, Spacer };

/** Simple struct for nodes used in the Algorithm X's data structure.
 * Root  : defines [left|right]
 * Header: defines [left|right|up|down|length]
 * Node  : defines [up|down|top]
 * Spacer: defines [up|down]
 */
struct Node {

  NodeType type = NodeType::Node;
  /// Index to left node
  int32_t left = -1;
  /// Index to right node
  int32_t right = -1;
  /// Index to up node
  int32_t up = -1;
  /// Index to up node
  int32_t down = -1;
  /// Index to header node
  int32_t header = -1;
  /// Amount of nodes that this header contains
  int32_t length = -1;

  /** Constructs a human-readable string of the type of node
   * @return The type as a string
   */
  std::string getTypeString() const;
};