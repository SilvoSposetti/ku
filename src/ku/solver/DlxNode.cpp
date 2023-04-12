#include "DlxNode.h"

DlxNode::DlxNode(DlxNodeType type)
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
}
