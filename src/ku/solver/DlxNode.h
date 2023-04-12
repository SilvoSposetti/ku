#pragma once

#include <string>
#include <vector>

/** Enum class defining the type of a DlxNode
 */
enum class DlxNodeType : uint8_t { Root, Header, Node, Spacer };

/** Simple struct for nodes used in the DlxMatrix data structure.
 */
struct DlxNode {

  DlxNode(DlxNodeType type);

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
