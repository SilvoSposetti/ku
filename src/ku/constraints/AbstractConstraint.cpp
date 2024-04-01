#include "AbstractConstraint.h"

AbstractConstraint::AbstractConstraint(ConstraintType type, const std::string& name, const std::string& description)
    : type(type)
    , name(name)
    , description(description) {}

const ConstraintType AbstractConstraint::getType() const {
  return type;
}

const std::string& AbstractConstraint::getName() const {
  return name;
}

const std::string& AbstractConstraint::getDescription() const {
  return description;
}

std::vector<std::tuple<int32_t, int32_t, Sudo::Digit>> AbstractConstraint::optionsOrdered() {
  std::vector<std::tuple<int32_t, int32_t, Sudo::Digit>> result(Sudo::INDICES.size() * Sudo::INDICES.size() *
                                                                Sudo::SUDO_DIGITS.size());
  int32_t counter = 0;
  for (const auto& rowIndex : Sudo::INDICES) {
    for (const auto& columnIndex : Sudo::INDICES) {
      for (const auto& digit : Sudo::SUDO_DIGITS) {
        result[counter] = std::make_tuple(rowIndex, columnIndex, digit);
        counter++;
      }
    }
  }
  return result;
}

