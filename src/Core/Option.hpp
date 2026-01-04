#pragma once

#include "FixedCapacityArray.hpp"
#include "OptionId.hpp"

template <OptionId N>
using Option = FixedCapacityArray<OptionId, N>;
