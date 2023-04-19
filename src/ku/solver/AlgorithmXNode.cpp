#include "AlgorithmXNode.h"

AlgorithmXNode::AlgorithmXNode(AlgorithmXNodeType type)
    : type(type) {}

std::string AlgorithmXNode::getTypeString() const {
  switch (type) {
  case AlgorithmXNodeType::Root:
    return "root  ";
  case AlgorithmXNodeType::Header:
    return "header";
  case AlgorithmXNodeType::Node:
    return "node  ";
  case AlgorithmXNodeType::Spacer:
    return "spacer";
  default:
    return "node  ";
  }
};