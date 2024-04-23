#pragma once
#include "../Sudo.h"

struct OptionData {
  OptionData() = default;

  OptionData(int32_t indexI, int32_t indexJ, Sudo::Digit digit);

  int32_t indexI = -1;
  int32_t indexJ = -1;
  Sudo::Digit digit = Sudo::Digit::NONE;
};