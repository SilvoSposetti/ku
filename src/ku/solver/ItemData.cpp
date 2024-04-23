#include "ItemData.h"

ItemData::ItemData(const std::string& name, bool isPrimary, int32_t itemId)
    : name(name)
    , isPrimary(isPrimary)
    , itemId(itemId) {}