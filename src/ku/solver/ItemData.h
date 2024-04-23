#pragma once
#include <string>

struct ItemData {
  ItemData() = default;

  ItemData(const std::string& name, bool isPrimary, int32_t itemId);

  std::string name;
  bool isPrimary = true;
  int32_t itemId = -1;
};
