#pragma once

#include "../utilities/FixedCapacityArray.hpp"
#include "OptionId.hpp"

template <OptionId N>
using Option = FixedCapacityArray<OptionId, N>;
