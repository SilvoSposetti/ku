#include "AbstractConstraint.h"

AbstractConstraint::AbstractConstraint(ConstraintType type, const std::string& name, const std::string& description)
    : type(type)
    , name(name)
    , description(description) {}

const std::string& AbstractConstraint::getName() const {
  return name;
}

const std::string& AbstractConstraint::getDescription() const {
  return description;
}

bool AbstractConstraint::isItemPrimary(int32_t itemId) const {
  return true;
};

std::vector<std::vector<int32_t>> AbstractConstraint::getItems() const {
  std::vector<std::vector<int32_t>> result(Sudo::INDICES.size() * Sudo::INDICES.size() * Sudo::SUDO_DIGITS.size());
  const int32_t itemsAmount = getItemsAmount();
  int32_t counter = 0;
  for (const auto& rowIndex : Sudo::INDICES) {
    for (const auto& columnIndex : Sudo::INDICES) {
      for (const auto& digit : Sudo::SUDO_DIGITS) {
        for (int32_t itemId = 0; itemId < itemsAmount; itemId++) {
          if (computeConstraint(digit, rowIndex, columnIndex, itemId)) {
            result[counter].push_back(itemId);
          }
        }
        counter++;
      }
    }
  }
  return result;
};

bool AbstractConstraint::isSecondaryItemsOnly() const {
  bool isSecondaryItemsOnly = true;
  for (int32_t i = 0; i < getItemsAmount(); i++) {
    isSecondaryItemsOnly &= !isItemPrimary(i);
    if (!isSecondaryItemsOnly) {
      return false;
    }
  }
  return isSecondaryItemsOnly;
}
