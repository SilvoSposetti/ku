#include "DlxNode.h"

DlxNode::DlxNode(DlxNodeType type)
    : type(type) {}

std::string DlxNode::getTypeString() const {
  switch (type) {
  case DlxNodeType::Root:
    return "root  ";
  case DlxNodeType::Header:
    return "header";
  case DlxNodeType::Node:
    return "node  ";
  case DlxNodeType::Spacer:
    return "spacer";
  default:
    return "node  ";
  }
};