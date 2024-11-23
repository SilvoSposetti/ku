#include "Sudo.hpp"

std::vector<std::vector<Sudo::Digit>> Sudo::emptyField() {
  return std::vector<std::vector<Sudo::Digit>>(Sudo::MAX_DIGIT,
                                               std::vector<Sudo::Digit>(Sudo::MAX_DIGIT, Sudo::Digit::NONE));
}

std::vector<std::vector<bool>> Sudo::fullGivenMask() {
  return std::vector<std::vector<bool>>(Sudo::MAX_DIGIT, std::vector<bool>(Sudo::MAX_DIGIT, true));
}

std::vector<std::vector<bool>> Sudo::emptyGivenMask() {
  return std::vector<std::vector<bool>>(Sudo::MAX_DIGIT, std::vector<bool>(Sudo::MAX_DIGIT, false));
}
