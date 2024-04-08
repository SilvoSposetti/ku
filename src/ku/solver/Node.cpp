#include "Node.h"

std::string Node::getTypeString() const {
  switch (type) {
  case NodeType::Root:
    return "root  ";
  case NodeType::Header:
    return "header";
  case NodeType::Node:
    return "node  ";
  case NodeType::Spacer:
    return "spacer";
  default:
    return "node  ";
  }
};