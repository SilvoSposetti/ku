#include "ItemData.h"

ItemData::ItemData(const std::string& constraintName, bool isPrimary, int32_t itemId)
    : constraintName(constraintName)
    , isPrimary(isPrimary)
    , itemId(itemId) {}